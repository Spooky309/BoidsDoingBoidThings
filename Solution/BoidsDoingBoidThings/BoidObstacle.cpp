#include "BoidObstacle.h"
#include "imgui.h"
#include "Entity.h"
#include "Engine.h"
#include <iostream>
BoidObstacle::BoidObstacle() : m_fPosX(0.0f), m_fPosY(0.0f), m_fPosZ(0.0f),m_fRadius(0.0f),m_iMyID(0),m_sImguiTitle("")
{
	m_modelAsset = Engine::Instance().GetAssetManager().LoadAsset<ModelAsset>("asset/obstacle.obj");
	m_selectedTextureAsset = Engine::Instance().GetAssetManager().LoadAsset<TextureAsset>("asset/obstacle_selected.png");
	m_deselectedTextureAsset = Engine::Instance().GetAssetManager().LoadAsset<TextureAsset>("asset/obstacle_unselected.png");
	glGenVertexArrays(1, &m_iVao);
	glBindVertexArray(m_iVao);
	glGenBuffers(1, &m_iVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVbo);
	GLfloat* fptr = nullptr;
	if (m_modelAsset->GetAnimation("", &fptr))
	{
		glBufferData(GL_ARRAY_BUFFER, m_modelAsset->GetModelDataSize() * sizeof(GLfloat), fptr, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}
void BoidObstacle::Update()
{
	if (!m_iMyID)
	{
		m_iMyID = GetParentEntity()->GetEntityID();
		m_sImguiTitle = "Obstacle: ID " + std::to_string(m_iMyID);
	}
	DrawImgui();
	RenderArgs args;
	args.model_matrix = GetParentEntity()->GetTransformMatrix();
	args.texture_id = m_deselectedTextureAsset->GetGLTexture();
	args.vao = m_iVao;
	args.vert_count = m_modelAsset->GetTriangleCount() * 3;
	Engine::Instance().GetRenderer3D().PushRender(args);
}

void BoidObstacle::DrawImgui()
{
	ImGui::Begin(m_sImguiTitle.c_str());
	ImGui::InputFloat("X Position", &m_fPosX, 0.5f, 1.0f, 1);
	ImGui::InputFloat("Y Position", &m_fPosY, 0.5f, 1.0f, 1);
	ImGui::InputFloat("Z Position", &m_fPosZ, 0.5f, 1.0f, 1);
	ImGui::SliderFloat("Radius", &m_fRadius, 1.0f, 500.0f);
	ImGui::End();
	GetParentEntity()->SetPosition(glm::vec3(m_fPosX, m_fPosY, m_fPosZ));
	GetParentEntity()->SetScale(glm::vec3(m_fRadius, m_fRadius, m_fRadius));
}