#include "InputSystem.h"

InputSystem* InputSystem::instance = nullptr;

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	InputSystem* inputSystem = InputSystem::GetInstance();

	if (inputSystem)
	{
		inputSystem->HandleFramebufferResize(width, height);
	}
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	InputSystem* inputSystem = InputSystem::GetInstance();

	if (inputSystem)
	{
		inputSystem->HandleMouseButtonInput(button, action, mods);
	}
}

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputSystem* inputSystem = InputSystem::GetInstance();

	if (inputSystem)
	{
		inputSystem->HandleCursorPositionInput(xpos, ypos);
	}
}

static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputSystem* inputSystem = InputSystem::GetInstance();

	if (inputSystem)
	{
		inputSystem->HandleMouseScrollInput(xoffset, yoffset);
	}
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		return;
	}

	InputSystem* inputSystem = InputSystem::GetInstance();

	if (inputSystem)
	{
		inputSystem->HandleKeyInput(key, scancode, action, mods);
	}
}

InputSystem::InputSystem()
{
	this->window = nullptr;
	this->camera = nullptr;

	this->lastX = 0;
	this->lastY = 0;

	this->firstMouse = true;
}

void InputSystem::Initialize(Window* window, RenderingSystem* renderingSystem)
{
	this->window = window;
	this->renderingSystem = renderingSystem;
	this->camera = renderingSystem->GetCamera();

	this->lastX = window->GetWidth() / 2.0f;
	this->lastY = window->GetHeight() / 2.0f;

	// Register callback functions for GLFW events.
	glfwSetFramebufferSizeCallback(this->window->GetGLFWWindow(), FramebufferSizeCallback);
	glfwSetKeyCallback(this->window->GetGLFWWindow(), KeyCallback);
	glfwSetCursorPosCallback(this->window->GetGLFWWindow(), CursorPositionCallback);
	glfwSetScrollCallback(this->window->GetGLFWWindow(), ScrollCallback);
	glfwSetMouseButtonCallback(this->window->GetGLFWWindow(), MouseButtonCallback);
}

void InputSystem::Update()
{
	glfwPollEvents();
}

void InputSystem::HandleKeyInput(int key, int scancode, int action, int mods)
{
	Time* time = Time::GetInstance();

	if (key == GLFW_KEY_W)
		camera->ProcessKeyboard(FORWARD, time->GetDeltaTime());
	if (key == GLFW_KEY_S)
		camera->ProcessKeyboard(BACKWARD, time->GetDeltaTime());
	if (key == GLFW_KEY_D)
		camera->ProcessKeyboard(RIGHT, time->GetDeltaTime());
	if (key == GLFW_KEY_A)
		camera->ProcessKeyboard(LEFT, time->GetDeltaTime());
}

void InputSystem::HandleMouseButtonInput(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1)
		std::cout << "Left mouse button was pressed!" << std::endl;
	if (button == GLFW_MOUSE_BUTTON_2)
		std::cout << "Right mouse button was pressed!" << std::endl;
}

void InputSystem::HandleCursorPositionInput(double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (this->firstMouse)
	{
		this->lastX = xpos;
		this->lastY = ypos;
		this->firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	this->lastX = xpos;
	this->lastY = ypos;

	this->camera->ProcessMouseMovement(xoffset, yoffset);
}

void InputSystem::HandleMouseScrollInput(double xoffset, double yoffset)
{
	float ydiff = static_cast<float>(yoffset);

	this->camera->ProcessMouseScroll(ydiff);
}

void InputSystem::HandleFramebufferResize(int width, int height)
{
	this->renderingSystem->ResizeFramebuffer(width, height);
}