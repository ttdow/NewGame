#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

class Window
{
private:

	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	GLFWwindow* window;

public:
	
	Window();
	~Window();

	bool Initialize();
	bool ShouldClose();

	int GetWidth();
	int GetHeight();

	GLFWwindow* GetGLFWWindow();
};