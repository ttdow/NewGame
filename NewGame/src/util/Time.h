#pragma once

#include "glfw/glfw3.h"

class Time
{
private:

	static Time* instance;

	double previousTime;
	float deltaTime;

	Time();

	Time(const Time&) = delete;
	Time& operator=(const Time&) = delete;

public:

	static Time* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new Time();
		}

		return instance;
	}

	void Update();
	float GetDeltaTime();
	static float GetTime();
};