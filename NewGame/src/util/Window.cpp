#include "Window.h"

Window::Window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4); // MSAA
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	this->SCR_WIDTH = mode->width;
	this->SCR_HEIGHT = mode->height;

	this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "NewGame", monitor, NULL);
}

Window::~Window()
{
	glfwTerminate();
}

bool Window::Initialize()
{
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	
	// Turn off vsync.
	glfwSwapInterval(0);

	// Eat mouse.
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	return true;
}

bool Window::ShouldClose()
{
	if (glfwWindowShouldClose(this->window))
		return true;
	else
		return false;
}

int Window::GetWidth()
{
	return this->SCR_WIDTH;
}

int Window::GetHeight()
{
	return this->SCR_HEIGHT;
}

GLFWwindow* Window::GetGLFWWindow()
{
	return this->window;
}