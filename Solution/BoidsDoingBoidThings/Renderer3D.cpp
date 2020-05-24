#include "Renderer3D.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>

void Renderer3D::Init()
{
	// load shaders
	GLuint fshad; // same for both
	GLuint vshad;
	GLuint inst_vshad;
	fshad = glCreateShader(GL_FRAGMENT_SHADER);
	vshad = glCreateShader(GL_VERTEX_SHADER);
	inst_vshad = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream file_fragment_shader("asset/standard3d.fs");
	std::ifstream file_vertex_shader("asset/standard3d.vs");
	std::ifstream file_inst_vertex_shader("asset/instanced3d.vs");
	// read them all into strings
	std::string str_fragment_shader((std::istreambuf_iterator<char>(file_fragment_shader)), std::istreambuf_iterator<char>());
	std::string str_vertex_shader((std::istreambuf_iterator<char>(file_vertex_shader)), std::istreambuf_iterator<char>());
	std::string str_inst_vertex_shader((std::istreambuf_iterator<char>(file_inst_vertex_shader)), std::istreambuf_iterator<char>());
	const char* cs_frag = str_fragment_shader.c_str();
	const char* cs_vert = str_vertex_shader.c_str();
	const char* cs_ivert = str_inst_vertex_shader.c_str();
	glShaderSource(fshad, 1, &cs_frag, nullptr);
	glCompileShader(fshad);
	glShaderSource(vshad, 1, &cs_vert, nullptr);
	glCompileShader(vshad);
	glShaderSource(inst_vshad, 1, &cs_ivert, nullptr);
	glCompileShader(inst_vshad);
	m_iRendShader = glCreateProgram();
	// Set up the normal rendering shader, find out where the relevant uniforms are
	// we store the uniforms because they're standardized so we don't have to get them every time we want to update one of them
	glAttachShader(m_iRendShader, vshad);
	glAttachShader(m_iRendShader, fshad);
	glLinkProgram(m_iRendShader);
	glUseProgram(m_iRendShader);
	m_iRendShaderModelMatLoc = glGetUniformLocation(m_iRendShader, "model");
	m_iRendShaderViewMatLoc = glGetUniformLocation(m_iRendShader, "view");
	m_iRendShaderProjMatLoc = glGetUniformLocation(m_iRendShader, "proj");

	// Ditto, for the instanced rendering shader
	m_iInstRendShader = glCreateProgram();
	glAttachShader(m_iInstRendShader, inst_vshad);
	glAttachShader(m_iInstRendShader, fshad);
	glLinkProgram(m_iInstRendShader);
	glUseProgram(m_iInstRendShader);
	m_iInstRendShaderViewMatLoc = glGetUniformLocation(m_iInstRendShader, "view");
	m_iInstRendShaderProjMatLoc = glGetUniformLocation(m_iInstRendShader, "proj");

	// Clear the current program, not sure if this is necessary but let's do it anyway.
	glUseProgram(0);

	glDeleteShader(fshad);
	glDeleteShader(vshad);
	glDeleteShader(inst_vshad);
}

void Renderer3D::Render()
{
	glUseProgram(m_iRendShader);
	while (!m_queuedRenders.empty())
	{
		RenderArgs thisRend = m_queuedRenders.front();

		// render it

		m_queuedRenders.pop();
	}
	glUseProgram(m_iInstRendShader);
	while (!m_queuedInstancedRenders.empty())
	{
		InstancedRenderArgs thisRend = m_queuedInstancedRenders.front();

		// render it

		m_queuedInstancedRenders.pop();
	}
}

void Renderer3D::PushRender(const RenderArgs& args)
{
	m_queuedRenders.push(args);
}

void Renderer3D::PushInstancedRender(const InstancedRenderArgs& args)
{
	m_queuedInstancedRenders.push(args);
}

void Renderer3D::UpdateViewMatrix(const glm::mat4& newView)
{
	glUseProgram(m_iRendShader);
	glUniformMatrix4fv(m_iRendShaderViewMatLoc, 1, GL_FALSE, glm::value_ptr(newView));
	glUseProgram(m_iInstRendShader);
	glUniformMatrix4fv(m_iInstRendShaderViewMatLoc, 1, GL_FALSE, glm::value_ptr(newView));
}