#pragma once

#include <glm/glm.hpp>

class Particle
{
public:

	glm::vec3 position;
	glm::vec3 velocity;

	glm::vec4 color;

	float life;

	Particle();
	Particle(glm::vec3 position, glm::vec4 color, glm::vec3 velocity, float life);
};