#pragma once

#include <glfw/glfw3.h>

#include <iostream>

class Profiler
{
private:

	double previousTime;
	int frameCount;

public:
	
	Profiler();
	void Update();
};