#pragma once

#include <glm/glm.hpp>

#include <vector>

class Triangle
{
public:

	glm::vec3 v0, v1, v2;

	unsigned int i0, i1, i2;

	glm::vec3 e0, e1;

	glm::vec3 normal;

};