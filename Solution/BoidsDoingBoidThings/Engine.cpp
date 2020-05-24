#include "Engine.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Engine::Engine() : m_pWindow(nullptr), m_dLastTime(0.0), m_dDeltaTime(0.0), m_pEntityWorld(new EntityWorld())
{
}

int Engine::Go()
{
	if (!glfwInit())
	{
		std::cout << "GLFW Initialization Failed\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // I remember this being necessary for macOS.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_pWindow = glfwCreateWindow(1280, 720, "dont crash", nullptr, nullptr);
	if (!m_pWindow)
	{
		std::cout << "Couldn't get a window from GLFW\n";
		return -2;
	}
	glfwMakeContextCurrent(m_pWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1); // let's get some vsync
	glClearColor(0.2f, 0.0f, 0.4f, 1.0f); // a good colour
	// make the rendering not terrible
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// SET CALLBACKS HERE, OTHERWISE IMGUI WILL OVERRIDE THEM

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
	ImGui_ImplOpenGL3_Init("#version 410 core");
	
	m_pEntityWorld->Init();
	while (!glfwWindowShouldClose(m_pWindow))
	{
		m_dLastTime = glfwGetTime();
		Update();
		m_dDeltaTime = glfwGetTime() - m_dLastTime;
	}

	Deinit();
}

void Engine::Update()
{
	// Updating
	glfwPollEvents();

	// Rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// make sure ui draws last
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Settings");
	ImGui::Text("thanks gonzo");
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(m_pWindow);
}

void Engine::Deinit()
{

}