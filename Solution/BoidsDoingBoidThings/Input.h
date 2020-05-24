#pragma once
#ifndef H_INPUT
#define H_INPUT
#include <glfw/glfw3.h>
class Input
{
public:
	inline double GetMousePosX() const;
	inline double GetMousePosY() const;
	inline bool   GetMouseButton(int button) const;
	inline bool   GetMouseButtonDown(int button) const;
	inline bool   GetMouseButtonUp(int button) const;
	inline bool   GetKey(int key) const;
	inline bool   GetKeyDown(int key) const;
	inline bool   GetKeyUp(int key) const;
	friend class  Engine;
private:
	void          Init(GLFWwindow* wind);
	void          Update();
	// GLFW needs static functions to callback, so I route them into an instance of this class
	static void   glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void   glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void   glfwMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	void          intKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void          intMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void          intMousePositionCallback(GLFWwindow* window, double xpos, double ypos);

	static Input* s_iPtr;
	unsigned char m_bKeys[512];
	unsigned char m_bLastKeys[512];
	unsigned char m_bBtns[16];
	unsigned char m_bLastBtns[16];
	double m_dXPos;
	double m_dYPos;

	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
};

#include "Input.inl"
#endif