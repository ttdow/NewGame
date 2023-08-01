#include "Profiler.h"

Profiler::Profiler()
{
	this->previousTime = glfwGetTime();
	this->frameCount = 0;
}

void Profiler::Update()
{
	double currentTime = glfwGetTime();
	double dTime = currentTime - this->previousTime;
	this->frameCount++;

	if (dTime >= 1.0)
	{
		double fps = this->frameCount / dTime;
		std::cout << "FPS: " << fps << std::endl;
		this->frameCount = 0;
		this->previousTime = currentTime;
	}
}