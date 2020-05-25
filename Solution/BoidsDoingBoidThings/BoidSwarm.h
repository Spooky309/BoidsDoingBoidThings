#pragma once
#ifndef H_BOIDSWARM
#define H_BOIDSWARM
#include "Component.h"
#include <glm/glm.hpp>
#include "ModelAsset.h"
#include "TextureAsset.h"
// The position of the parent entity of this component acts as the "target" for the boids

class BoidSwarm : public Component
{
public:
	BoidSwarm();
	void Update() override;
	void DrawImgui();
private:
	void PushRender();
	void SetupRendering();
	glm::vec3 TargetForce(const glm::vec3& pos);
	glm::vec3 AvoidForce(const glm::vec3& pos, std::size_t numBoidsInRange);
	glm::vec3 AlignmentForce(const glm::vec3& pos, std::size_t numBoidsInRange);
	glm::vec3 CoalesceForce(const glm::vec3& pos, std::size_t numBoidsInRange);
	glm::vec3 SteerTowards(const glm::vec3& currentVel, const glm::vec3& targetVel);
	// This function returns a list of indices into m_aBoidTransforms for all the ones in range of a given point
	// based on m_fBoidViewDistance
	std::size_t BoidsInRange(const glm::vec3& pos);
	// I wanna keep this allocated to avoid unnecessary re-allocations
	std::size_t* m_aBoidRangeArray = nullptr;

	unsigned int m_iNumBoids;
	unsigned int m_iLastNumBoids;
	int m_iNewNumBoids;
	ModelAsset* m_boidModel;
	TextureAsset* m_boidTexture;

	glm::vec3* m_aBoidVelocities;
	glm::vec3* m_aBoidAccelerations;
	glm::mat4* m_aBoidTransforms;

	// Variables to do with the simulation
	float m_fBoidDrag;
	float m_fBoidDesiredSpeed;
	float m_fBoidMaxForce;
	float m_fMovementCircleRadius;
	float m_fBoidViewDistance = 400.0f;
	float m_fBoidSeparationDistance = 4.0f;
	float m_fBoidConstraintWidth = 500.0f;
	float m_fBoidConstraintHeight = 500.0f;
	float m_fBoidConstraintLength = 500.0f;
	// Weights on forces
	float m_fBoidTargetWeight = 0.5f;
	float m_fBoidAvoidWeight = 1.0f;
	float m_fBoidAlignmentWeight = 0.25f;
	float m_fBoidCoalesceWeight = 1.0f;

	bool m_bUseTarget = false;
	bool m_bUsePlayerAsTarget = false;
	std::weak_ptr<Entity> m_pTarget;
	bool m_bUseAvoid = true;
	bool m_bUseAlign = true;
	bool m_bUseCoalesce = true;

	GLuint m_iVao;
	GLuint m_iModelVbo;
	GLuint m_iInstanceVbo;
};

#endif