#pragma once

#include <glm/glm.hpp>

#include <vector>

class OctreeNode
{
public:

	glm::vec3 min;
	glm::vec3 max;

	std::vector<OctreeNode> children;

	OctreeNode()
	{
		this->min = glm::vec3(0.0f, 0.0f, 0.0f);
		this->max = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	OctreeNode(glm::vec3 min, glm::vec3 max)
	{
		this->min = min;
		this->max = max;
	}
};