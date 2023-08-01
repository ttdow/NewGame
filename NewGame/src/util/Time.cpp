#include "Time.h"

Time* Time::instance = nullptr;

Time::Time()
{
	this->previousTime = glfwGetTime();
	this->deltaTime = 0;
}

void Time::Update()
{
	double currentTime = glfwGetTime();
	double dTime = currentTime - this->previousTime;
	this->previousTime = currentTime;

	this->deltaTime = static_cast<float>(dTime);
}

float Time::GetDeltaTime()
{
	return this->deltaTime;
}

float Time::GetTime()
{
	return static_cast<float>(glfwGetTime());
}