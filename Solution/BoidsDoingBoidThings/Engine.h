#pragma once
#ifndef H_ENGINE
#define H_ENGINE
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "EntityWorld.h"
#include "Input.h"
// Engine is a singleton class that represents a global point of access for all of the critical systems
// It's not great but it's cleaner than passing pointers around everywhere.

class Engine
{
public:
	static Engine& Instance()
	{
		static Engine _inst;
		return _inst;
	}
	int Go(); // This function is called by main to make it start going
	inline double GetDeltaTime();
	inline GLFWwindow* GetWindow();
	inline Input& GetInput();
private:
	void Update(); // This is responsible for telling all the entities to update and the renderers to render afterwards
	void Deinit();
	double m_dLastTime; // stores the time the last frame began
	double m_dDeltaTime; // stores the time the last frame took to render
	Input m_Input;
	GLFWwindow* m_pWindow;
	EntityWorld* m_pEntityWorld;
	Engine();
	~Engine() = default;
	Engine(const Engine&) = delete;
};

#include "Engine.inl"
#endif