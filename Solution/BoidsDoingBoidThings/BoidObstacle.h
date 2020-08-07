#pragma once
#ifndef H_BOIDOBSTACLE
#define H_BOIDOBSTACLE
#include "Component.h"
#include "ModelAsset.h"
#include "TextureAsset.h"
#include <glad/glad.h>
class BoidObstacle : public Component
{
public:
	BoidObstacle();
	void Update() override;
	void DrawImgui();
private:
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	float m_fRadius;
	unsigned int m_iMyID;
	std::string m_sImguiTitle;
	ModelAsset* m_modelAsset;
	TextureAsset* m_selectedTextureAsset;
	TextureAsset* m_deselectedTextureAsset;
	GLuint m_iVbo;
	GLuint m_iVao;
};

#endif