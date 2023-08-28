#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

class Window
{
private:

	unsigned int SCR_WIDTH = 1280;
	unsigned int SCR_HEIGHT = 720;

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