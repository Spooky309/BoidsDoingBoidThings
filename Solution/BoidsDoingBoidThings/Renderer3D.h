#pragma once
#ifndef H_RENDERER3D
#define H_RENDERER3D
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <queue>

struct RenderArgs
{
	GLuint texture_id;
	GLuint vao;
	GLuint vert_count;
	glm::mat4 model_matrix;
};

struct InstancedRenderArgs
{
	GLuint texture_id;
	GLuint vao;
	GLuint vert_count;
	GLuint num_instances;
};

class Renderer3D
{
public:
	friend class Engine;
	void PushRender(const RenderArgs& args);
	void PushInstancedRender(const InstancedRenderArgs& args);
	void UpdateViewMatrix(const glm::mat4& newView);
private:
	void Init();
	void Render();
	void UpdateRes(int newX, int newY);
	std::queue<RenderArgs> m_queuedRenders;
	std::queue<InstancedRenderArgs> m_queuedInstancedRenders;
	GLuint m_iRendShader;
	GLint m_iRendShaderModelMatLoc;
	GLint m_iRendShaderViewMatLoc;
	GLint m_iRendShaderProjMatLoc;
	GLuint m_iInstRendShader;
	GLint m_iInstRendShaderViewMatLoc;
	GLint m_iInstRendShaderProjMatLoc;
	Renderer3D() = default;
	~Renderer3D() = default;
	Renderer3D(const Renderer3D&) = delete;
};

#endif