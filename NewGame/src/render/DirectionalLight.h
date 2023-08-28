#pragma once

#include <glm/glm.hpp>

class DirectionalLight
{
public:

	glm::vec3 direction;

	glm::vec3 ambientIntensity;
	glm::vec3 diffuseIntensity;
	glm::vec3 specularIntensity;
};