#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "../render/RenderingSystem.h"
#include "../util/Window.h"
#include "../render/Camera.h"
#include "../util/Time.h"
#include "PlayerController.h"

class InputSystem
{
private:

	static InputSystem* instance;

	Window* window;
	Camera* camera;
	RenderingSystem* renderingSystem;

	float lastX;
	float lastY;
	bool firstMouse;

	InputSystem();

	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

public:

	PlayerController* playerController;

	static InputSystem* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new InputSystem();
		}

		return instance;
	}

	void Initialize(Window* window, RenderingSystem* renderingSystem);
	void Update();
	
	void HandleKeyInput(int key, int scancode, int action, int mods);
	void HandleMouseButtonInput(int button, int action, int mods);
	void HandleCursorPositionInput(double xpos, double ypos);
	void HandleMouseScrollInput(double xoffset, double yoffset);
	void HandleFramebufferResize(int width, int height);
};