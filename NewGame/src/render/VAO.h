#pragma once

#include <glad/glad.h>

#include "VBO.h"
#include "Bone.h"

#include <iostream>

class VAO
{
private:


public:

	unsigned int id;

	bool animated;

	VAO();
	void Bind();
	void Unbind();
	void Setup();
};