#pragma once

#include <glm/glm.hpp>

class Ray
{
public:

	glm::vec3 origin;


	Ray(glm::vec3 origin, glm::vec3 direction)
	{
		this->origin = origin;
		this->direction = direction;
	}

	glm::vec3 GetDirection()
	{
		return glm::normalize(this->direction);
	}
	
private:

	glm::vec3 direction;
};