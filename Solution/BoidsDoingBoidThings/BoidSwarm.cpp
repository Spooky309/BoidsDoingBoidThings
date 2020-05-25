#include "BoidSwarm.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
BoidSwarm::BoidSwarm() :
	m_iNumBoids(10000),
	m_boidModel(nullptr),
	m_boidTexture(nullptr),
	m_iLastNumBoids(10000),
	m_iVao(0),
	m_iModelVbo(0),
	m_iInstanceVbo(0),
	m_aBoidPositions(nullptr),
	m_aBoidEulers(nullptr),
	m_aBoidTransforms(nullptr)
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
	if (m_aBoidPositions)
	{
		free(m_aBoidPositions);
	}
	if (m_aBoidEulers)
	{
		free(m_aBoidEulers);
	}
	if (m_aBoidTransforms)
	{
		free(m_aBoidTransforms);
	}
	if (m_iInstanceVbo)
	{
		glDeleteBuffers(1, &m_iInstanceVbo);
	}
	m_aBoidPositions = (glm::vec3*)malloc(sizeof(glm::vec3) * m_iNumBoids);
	m_aBoidEulers = (glm::vec3*)malloc(sizeof(glm::vec3) * m_iNumBoids);
	m_aBoidTransforms = (glm::mat4*)malloc(sizeof(glm::mat4) * m_iNumBoids);
	// initialize them all
	for (std::size_t i = 0; i < m_iNumBoids; i++)
	{
		m_aBoidPositions[i] = glm::vec3((float)i*2, 0.0f, 0.0f);
		m_aBoidEulers[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		m_aBoidTransforms[i] = glm::mat4(1.0f);
		m_aBoidTransforms[i] = glm::translate(m_aBoidTransforms[i], m_aBoidPositions[i]);
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
}

void BoidSwarm::Update()
{
	// first, we need to see if the number of boids has changed, if so, we need to re-create the arrays and VBO/VAO
	if (m_iNumBoids != m_iLastNumBoids)
	{
		m_iLastNumBoids = m_iNumBoids;
		SetupRendering();
	}
	// update all the boids and their transforms
	for (std::size_t i = 0; i < m_iNumBoids; i++)
	{
		glm::vec3 positionDelta(0);
		glm::vec3 eulerDelta(0);

		positionDelta.z = 1.0f * Engine::Instance().GetDeltaTime();

		m_aBoidPositions[i] += positionDelta;
		m_aBoidEulers[i] += eulerDelta;

		if (positionDelta.length() > 0.0f)
		{
			m_aBoidTransforms[i] = glm::translate(m_aBoidTransforms[i], positionDelta);
		}
		if (eulerDelta.length() > 0.0f)
		{
			m_aBoidTransforms[i] = glm::rotate(m_aBoidTransforms[i], eulerDelta.x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_aBoidTransforms[i] = glm::rotate(m_aBoidTransforms[i], eulerDelta.y, glm::vec3(0.0f, 1.0f, 0.0f));
			m_aBoidTransforms[i] = glm::rotate(m_aBoidTransforms[i], eulerDelta.z, glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_iInstanceVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_iNumBoids * sizeof(glm::mat4), m_aBoidTransforms); // I think it's better to push all the transforms than just the ones that updated
	// because that'd result in more gl calls
	// push the render to the rqueue
	PushRender();
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