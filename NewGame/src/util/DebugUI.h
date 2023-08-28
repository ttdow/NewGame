#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "../render/RenderingSystem.h"

#include <iostream>

class DebugUI
{
public:

	GLFWwindow* window;

	DebugUI(GLFWwindow* window);
	void Setup();
	void NewFrame(RenderingSystem* renderingSystem);
	void EndFrame();
	void Cleanup();
};

