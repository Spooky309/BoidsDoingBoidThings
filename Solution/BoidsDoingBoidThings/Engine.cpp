#include "Engine.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ViewControls.h"
#include "Entity.h"
#include "Camera.h"
#include "BoidSwarm.h"

Engine::Engine() : m_pWindow(nullptr), m_dLastTime(0.0), m_dDeltaTime(0.0)
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
	m_Input.Init(m_pWindow);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
	ImGui_ImplOpenGL3_Init("#version 410 core");
	
	m_rend3d.Init();

	m_pViewer = m_EntityWorld.CreateEntity("viewer");
	m_pViewer.lock()->AddComponent<ViewControls>(); // I dunno what this warning is on about
	m_pViewer.lock()->AddComponent<Camera>();

	m_pBoidSwarm = m_EntityWorld.CreateEntity("boidswarm");
	m_pViewer.lock()->AddComponent<BoidSwarm>();

	while (!glfwWindowShouldClose(m_pWindow))
	{
		m_dLastTime = glfwGetTime();
		Update();
		m_dDeltaTime = glfwGetTime() - m_dLastTime;
	}

	Deinit();
	return 0;
}

void Engine::Update()
{
	// Updating
	m_Input.Update(); // we first want to copy the keys and buttons from the last frame to the "lastframe" array, before we poll events (which will overwrite current ones)
	glfwPollEvents();
	m_EntityWorld.Update();

	// Rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_rend3d.Render();
	// make sure ui draws last
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Info");
	ImGui::Text("Viewer Position: %f, %f, %f", m_pViewer.lock()->GetPosition().x, m_pViewer.lock()->GetPosition().y, m_pViewer.lock()->GetPosition().z);
	ImGui::Text("Viewer Rotation: %f, %f, %f", m_pViewer.lock()->GetEuler().x, m_pViewer.lock()->GetEuler().y, m_pViewer.lock()->GetEuler().z);
	ImGui::Text("Viewer Forward: %f, %f, %f", m_pViewer.lock()->GetForward().x, m_pViewer.lock()->GetForward().y, m_pViewer.lock()->GetForward().z);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(m_pWindow);

	if (m_Input.GetKeyDown(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
	}
}

void Engine::Deinit()
{
	glfwTerminate();
}