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
	this->playerController->Reset();
	glfwPollEvents();
	this->playerController->Update(this->renderingSystem);
}

void InputSystem::HandleKeyInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			//this->renderingSystem->ChangeAnimation("walk");
			this->playerController->HandleInput(PlayerMovement::FORWARD);
		}
		else if (action == GLFW_RELEASE)
		{
			//this->renderingSystem->ChangeAnimation("idle");
		}
	}

	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			//this->renderingSystem->ChangeAnimation("walk");
			this->playerController->HandleInput(PlayerMovement::BACKWARD);
		}
		else if (action == GLFW_RELEASE)
		{
			//this->renderingSystem->ChangeAnimation("idle");
		}
	}

	if (key == GLFW_KEY_D)
	{
		this->playerController->HandleInput(PlayerMovement::RIGHT);
	}

	if (key == GLFW_KEY_A)
	{
		this->playerController->HandleInput(PlayerMovement::LEFT);
	}

	if (key == GLFW_KEY_E)
	{
		this->playerController->HandleInput(PlayerMovement::UP);
	}

	if (key == GLFW_KEY_Q)
	{
		this->playerController->HandleInput(PlayerMovement::DOWN);
	}

	if (key == GLFW_KEY_SPACE)
	{
		//this->renderingSystem->ChangeAnimation("jump");
		this->playerController->HandleInput(PlayerMovement::JUMP);
	}

	if (key == GLFW_KEY_R)
	{
		//this->renderingSystem->ChangeAnimation("attack");
	}

	//if (key == GLFW_KEY_0)
		//this->renderingSystem->ChangeAnimation("idle");
	//if (key == GLFW_KEY_1)
		//this->renderingSystem->ChangeAnimation("walk");
}

void InputSystem::HandleMouseButtonInput(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		//this->renderingSystem->ChangeAnimation("attack");

		if (action == GLFW_PRESS)
		{
			this->camera->moveCamera = true;
		}
		else if (action == GLFW_RELEASE)
		{
			this->camera->moveCamera = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_2)
	{
		if (action == GLFW_PRESS)
		{
			this->camera->freeCamera = true;
			this->playerController->freeCamera = true;
		}
		else if (action == GLFW_RELEASE)
		{
			this->camera->freeCamera = false;
			this->playerController->freeCamera = false;
		}
	}
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
	this->playerController->HandleMouseInput(xoffset, yoffset);
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