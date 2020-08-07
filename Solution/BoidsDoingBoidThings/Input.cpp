#include "Input.h"
#include <string.h>

Input* Input::s_iPtr;

void Input::Init(GLFWwindow* wind)
{
	s_iPtr = this;
	glfwSetKeyCallback(wind, &Input::glfwKeyCallback);
	glfwSetMouseButtonCallback(wind, &Input::glfwMouseButtonCallback);
	glfwSetCursorPosCallback(wind, &Input::glfwMousePositionCallback);
	memset(m_bKeys, 0, 512);
	memset(m_bLastKeys, 0, 512);
	memset(m_bBtns, 0, 16);
	memset(m_bLastBtns, 0, 16);
	m_dXPos = 0.0;
	m_dYPos = 0.0;
}

void Input::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (s_iPtr)
	{
		s_iPtr->intKeyCallback(window, key, scancode, action, mods);
	}
}
void Input::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (s_iPtr)
	{
		s_iPtr->intMouseButtonCallback(window, button, action, mods);
	}
}
void Input::glfwMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (s_iPtr)
	{
		s_iPtr->intMousePositionCallback(window, xpos, ypos);
	}
}

void Input::intKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		m_bKeys[key] = 1;
	}
	else if (action == GLFW_RELEASE)
	{
		m_bKeys[key] = 0;
	}
}
void Input::intMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		m_bBtns[button] = 1;
	}
	else if (action == GLFW_RELEASE)
	{
		m_bBtns[button] = 0;
	}
}
void Input::intMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	m_dXPos = xpos;
	m_dYPos = ypos;
}

void Input::Update()
{
	// copy current keys and buttons into last array, so they can be compared for GetKeyDown and GetKeyUp etc
	memcpy(m_bLastKeys, m_bKeys, 512);
	memcpy(m_bLastBtns, m_bBtns, 16);
}