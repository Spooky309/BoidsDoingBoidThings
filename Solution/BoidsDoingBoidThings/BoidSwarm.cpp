#include "BoidSwarm.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Entity.h"
#include <random>
#include "imgui.h"
#include <thread>

//helper macro so i can stop typing the same thing over and over again
#define VECZERO(vec) (vec.x == 0.0f && vec.y == 0.0f && vec.z == 0.0f)

BoidSwarm::BoidSwarm() :
	m_iNumBoids(500),
	m_boidModel(nullptr),
	m_boidTexture(nullptr),
	m_iLastNumBoids(500),
	m_iVao(0),
	m_iModelVbo(0),
	m_iInstanceVbo(0),
	m_aBoidVelocities(nullptr),
	m_aBoidAccelerations(nullptr),
	m_aBoidTransforms(nullptr),
	m_fMovementCircleRadius(100.0f),
	m_fBoidDrag(0.01f),
	m_fBoidDesiredSpeed(50.0f),
	m_fBoidMaxForce(0.05f),
	m_iNewNumBoids(500)
{
	m_boidModel = Engine::Instance().GetAssetManager().LoadAsset<ModelAsset>("asset/boid.obj");
	m_boidTexture = Engine::Instance().GetAssetManager().LoadAsset<TextureAsset>("asset/boid.png");
	glGenVertexArrays(1, &m_iVao);
	glBindVertexArray(m_iVao);
	glGenBuffers(1, &m_iModelVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_iModelVbo);
	GLfloat* fptr = nullptr;
	if (m_boidModel->GetAnimation("", &fptr))
	{
		glBufferData(GL_ARRAY_BUFFER, m_boidModel->GetModelDataSize() * sizeof(GLfloat), fptr, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	SetupRendering();
}

void BoidSwarm::SetupRendering()
{
	// if we changed size, we need to free the old arrays
	if (m_aBoidVelocities)
	{
		free(m_aBoidVelocities);
	}
	if (m_aBoidAccelerations)
	{
		free(m_aBoidAccelerations);
	}
	if (m_aBoidTransforms)
	{
		free(m_aBoidTransforms);
	}
	if (m_iInstanceVbo)
	{
		glDeleteBuffers(1, &m_iInstanceVbo);
	}
	m_aBoidVelocities = (glm::vec3*)malloc(sizeof(glm::vec3) * m_iNumBoids);
	m_aBoidAccelerations = (glm::vec3*)malloc(sizeof(glm::vec3) * m_iNumBoids);
	m_aBoidTransforms = (glm::mat4*)malloc(sizeof(glm::mat4) * m_iNumBoids);
	// initialize them all
	for (std::size_t i = 0; i < m_iNumBoids; i++)
	{
		m_aBoidVelocities[i] = glm::vec3(0.0f);
		m_aBoidAccelerations[i] = glm::vec3(0.0f);
		m_aBoidTransforms[i] = glm::mat4(1.0f);
		// randomly generate position between -num_boids and num_boids
		int x = (((rand() % m_iNumBoids) * 2) - m_iNumBoids);
		int y = (((rand() % m_iNumBoids) * 2) - m_iNumBoids);
		int z = (((rand() % m_iNumBoids) * 2) - m_iNumBoids);
		m_aBoidTransforms[i] = glm::translate(m_aBoidTransforms[i], glm::vec3(x, y, z));
	}
	glGenBuffers(1, &m_iInstanceVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_iInstanceVbo);
	glBufferData(GL_ARRAY_BUFFER, m_iNumBoids * sizeof(glm::mat4), m_aBoidTransforms, GL_DYNAMIC_DRAW);
	glBindVertexArray(m_iVao);
	// Because we can only have up to a vec4 worth of information in one attrib, we have to split up the mat4
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	if (m_aBoidRangeArray)
	{
		free(m_aBoidRangeArray);
	}
	m_aBoidRangeArray = (std::size_t*)malloc(sizeof(std::size_t) * m_iNumBoids);
}

void BoidSwarm::Update()
{
	// first, we need to see if the number of boids has changed, if so, we need to re-create the arrays and VBO/VAO
	if (m_iNumBoids != m_iLastNumBoids)
	{
		m_iLastNumBoids = m_iNumBoids;
		SetupRendering();
	}
	// move the entity (which serves as a target) around in a big circle to make all the boids follow it
	// also gonna oscillate the y position because why not
	float time = (float)glfwGetTime();
	GetParentEntity()->SetPosition(glm::vec3(sin(time) * m_fMovementCircleRadius, sin(time) * m_fMovementCircleRadius, cos(time) * m_fMovementCircleRadius));
	// update all the boids and their transforms
	for (std::size_t i = 0; i < m_iNumBoids; i++)
	{
		glm::vec3 positionDelta(0);
		// make a decision
		glm::vec3* myPosition = (glm::vec3*)&m_aBoidTransforms[i][3]; // the fourth row of the matrix is our position
		glm::vec3 desiredVelocity(0);
		glm::vec3 target(0.0f);
		glm::vec3 avoid(0.0f);
		glm::vec3 align(0.0f);
		glm::vec3 coalesce(0.0f);
		std::size_t numBoidsInRange = 0;
		if (m_bUseAvoid || m_bUseAlign || m_bUseCoalesce)
		{
			numBoidsInRange = BoidsInRange(*myPosition);
		}
		if (m_bUseTarget)
		{
			target = TargetForce(*myPosition);
			if (!VECZERO(target))
			{
				target = glm::normalize(target);
				target *= m_fBoidDesiredSpeed;
				target = SteerTowards(m_aBoidVelocities[i], target);
			}
			desiredVelocity += target * m_fBoidTargetWeight;
		}
		if (m_bUseAvoid)
		{
			glm::vec3 avoid = AvoidForce(*myPosition, numBoidsInRange);
			if (!VECZERO(avoid))
			{
				avoid = glm::normalize(avoid);
				avoid *= m_fBoidDesiredSpeed;
				avoid = SteerTowards(m_aBoidVelocities[i], avoid);
			}
			desiredVelocity += avoid * m_fBoidAvoidWeight;
		}
		if (m_bUseAlign)
		{
			glm::vec3 align = AlignmentForce(*myPosition, numBoidsInRange);
			if (!VECZERO(align))
			{
				align = glm::normalize(align);
				align *= m_fBoidDesiredSpeed;
				align = SteerTowards(m_aBoidVelocities[i], align);
			}
			desiredVelocity += align * m_fBoidAlignmentWeight;
		}
		if (m_bUseCoalesce)
		{
			glm::vec3 coalesce = CoalesceForce(*myPosition, numBoidsInRange);
			if (!VECZERO(coalesce))
			{
				coalesce = glm::normalize(coalesce);
				coalesce *= m_fBoidDesiredSpeed;
				coalesce = SteerTowards(m_aBoidVelocities[i], coalesce);
			}
			desiredVelocity += coalesce * m_fBoidCoalesceWeight;
		}

		glm::vec3 steeringForce = desiredVelocity - m_aBoidVelocities[i];
		m_aBoidAccelerations[i] = steeringForce;
		if (m_aBoidAccelerations[i].length() > m_fBoidMaxForce)
		{
			m_aBoidAccelerations[i] = glm::normalize(m_aBoidAccelerations[i]);
			m_aBoidAccelerations[i] *= m_fBoidMaxForce;
		}
		// apply acceleration and velocity
		m_aBoidVelocities[i] += m_aBoidAccelerations[i];
		m_aBoidVelocities[i] *= ((1.0f - m_fBoidDrag));
		*myPosition += m_aBoidVelocities[i];
		m_aBoidAccelerations[i] = glm::vec3(0.0f);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_iInstanceVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_iNumBoids * sizeof(glm::mat4), m_aBoidTransforms); // I think it's better to push all the transforms than just the ones that updated
	// because that'd result in more gl calls
	// push the render to the rqueue
	PushRender();
}

glm::vec3 BoidSwarm::TargetForce(const glm::vec3& pos)
{
	glm::vec3 desiredVelocity = GetParentEntity()->GetPosition() - pos;
	if (desiredVelocity.length() > 0.0f)
	{
		desiredVelocity = glm::normalize(desiredVelocity);
		desiredVelocity *= m_fBoidDesiredSpeed;
	}
	return desiredVelocity;
}

glm::vec3 BoidSwarm::AvoidForce(const glm::vec3& pos, std::size_t numBoidsInRange)
{
	glm::vec3 desiredVelocity(0.0f);
	for (std::size_t i = 0; i < numBoidsInRange; i++)
	{
		glm::vec3* otherPos = (glm::vec3*)&m_aBoidTransforms[m_aBoidRangeArray[i]][3];
		glm::vec3 diff = pos - *otherPos;
		if (!VECZERO(diff))
		{
			float dist = diff.length();
			if (dist < m_fBoidSeparationDistance)
			{
				diff = glm::normalize(diff);
				diff /= dist;
				desiredVelocity += diff;
			}
		}
	}
	return desiredVelocity;
}
glm::vec3 BoidSwarm::AlignmentForce(const glm::vec3& pos, std::size_t numBoidsInRange)
{
	glm::vec3 sumVel(0.0f);
	for (std::size_t i = 0; i < numBoidsInRange; i++)
	{
		glm::vec3* otherVel = (glm::vec3*) & m_aBoidVelocities[m_aBoidRangeArray[i]];
		sumVel += *otherVel;
	}
	return sumVel;
}
glm::vec3 BoidSwarm::CoalesceForce(const glm::vec3& pos, std::size_t numBoidsInRange)
{
	// This is also really slow
	glm::vec3 avg(0.0f);
	// sum up the position of all the nearby boids
	for (std::size_t i = 0; i < numBoidsInRange; i++)
	{
		glm::vec3* otherPos = (glm::vec3*) & m_aBoidTransforms[m_aBoidRangeArray[i]][3];
		avg += *otherPos;
	}
	if (numBoidsInRange > 0)
	{
		avg /= (float)numBoidsInRange; // make it an average
		avg -= pos; // get the distance between that average and us
	}
	return avg; // we can let the main loop process this
}

glm::vec3 BoidSwarm::SteerTowards(const glm::vec3& currentVel, const glm::vec3& targetVel)
{
	return targetVel - currentVel;
}

std::size_t BoidSwarm::BoidsInRange(const glm::vec3& pos)
{
	std::size_t num_in_range = 0;
	std::size_t arr_it = 0;
	// this is INCREDIBLY slow, and limits the number of boids to be used with any range checked forces to a few thousand
	// even with multithreading, we could get that up to maybe 10000
	// it also tanks debug builds like a brick
	// I'm not even sure how I'd thread this right now.
	for (std::size_t i = 0; i < m_iNumBoids; i++)
	{
		glm::vec3* thePosition = (glm::vec3*) & m_aBoidTransforms[i][3];
		if (glm::abs(*thePosition - pos).length() < m_fBoidViewDistance)
		{
			num_in_range++;
			m_aBoidRangeArray[arr_it++] = i;
		}
	}
	return num_in_range;
}

void BoidSwarm::PushRender()
{
	// create the args struct to tell the renderer how we want to render
	InstancedRenderArgs args =
	{
		m_boidTexture->GetGLTexture(),
		m_iVao,
		m_boidModel->GetTriangleCount() * 3,
		m_iNumBoids
	};
	Engine::Instance().GetRenderer3D().PushInstancedRender(args);
}

void BoidSwarm::DrawImgui()
{
	float newTargetWeight = m_fBoidTargetWeight;
	float newAvoidWeight = m_fBoidAvoidWeight;
	float newAlignWeight = m_fBoidAlignmentWeight;
	float newCoalesceWeight = m_fBoidCoalesceWeight;
	ImGui::Begin("Boid Settings");
	ImGui::SliderFloat("Boid View Distance", &m_fBoidViewDistance, 1.0f, 1000.0f);
	ImGui::Separator();
	ImGui::Checkbox("Use Target", &m_bUseTarget);
	ImGui::SliderFloat("Target Force", &newTargetWeight, 0.0f, 1.0f);
	ImGui::Separator();
	ImGui::Checkbox("Use Avoid", &m_bUseAvoid);
	ImGui::SliderFloat("Avoid Force", &newAvoidWeight, 0.0f, 1.0f);
	ImGui::SliderFloat("Avoid Space", &m_fBoidSeparationDistance, 1.0f, m_fBoidViewDistance);
	ImGui::Separator();
	ImGui::Checkbox("Use Align", &m_bUseAlign);
	ImGui::SliderFloat("Alignment Force", &newAlignWeight, 0.0f, 1.0f);
	ImGui::Separator();
	ImGui::Checkbox("Use Coalesce", &m_bUseCoalesce);
	ImGui::SliderFloat("Coalesce Force", &newCoalesceWeight, 0.0f, 1.0f);
	ImGui::Separator();
	ImGui::SliderInt("Number of Boids", &m_iNewNumBoids, 1, 10000);
	ImGui::Separator();
	if (ImGui::Button("Apply and Restart"))
	{
		m_iNumBoids = m_iNewNumBoids;
	}
	ImGui::Text("You have to press this to update Boid Count\nYou probably don't want to go over 1,000 with all the options enabled.");

	ImGui::End();

	// if the sum of all the weights is above 1, we need to modify the ones that weren't changed by sum(all)-1.0/3
	float sumWeights = newTargetWeight + newAvoidWeight + newAlignWeight + newCoalesceWeight;
	if (sumWeights > 1.0f)
	{
		float sub = (sumWeights - 1.0f) / 3.0f;
		if (newTargetWeight != m_fBoidTargetWeight)
		{
			newAvoidWeight -= sub;
			newAlignWeight -= sub;
			newCoalesceWeight -= sub;
		}
		if (newAvoidWeight != m_fBoidAvoidWeight)
		{
			newTargetWeight -= sub;
			newAlignWeight -= sub;
			newCoalesceWeight -= sub;
		}
		if (newAlignWeight != m_fBoidAlignmentWeight)
		{
			newAvoidWeight -= sub;
			newTargetWeight -= sub;
			newCoalesceWeight -= sub;
		}
		if (newCoalesceWeight != m_fBoidCoalesceWeight)
		{
			newAvoidWeight -= sub;
			newAlignWeight -= sub;
			newTargetWeight -= sub;
		}
	}
	if (m_fBoidSeparationDistance > m_fBoidViewDistance)
	{
		m_fBoidSeparationDistance = m_fBoidViewDistance;
	}

	m_fBoidTargetWeight = newTargetWeight;
	m_fBoidAvoidWeight = newAvoidWeight;
	m_fBoidAlignmentWeight = newAlignWeight;
	m_fBoidCoalesceWeight = newCoalesceWeight;
}