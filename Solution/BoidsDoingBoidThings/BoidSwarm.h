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
private:
	void PushRender();
	void SetupRendering();
	unsigned int m_iNumBoids;
	unsigned int m_iLastNumBoids;
	ModelAsset* m_boidModel;
	TextureAsset* m_boidTexture;
	glm::vec3* m_aBoidPositions;
	glm::vec3* m_aBoidEulers;
	glm::mat4* m_aBoidTransforms;
	GLuint m_iVao;
	GLuint m_iModelVbo;
	GLuint m_iInstanceVbo;
};

#endif