#pragma once

#include <glm/glm.hpp>

class PointLight
{
public:

	glm::vec3& position;

	glm::vec3 ambientIntensity;
	glm::vec3 diffuseIntensity;
	glm::vec3 specularIntensity;

	float constant;
	float linear;
	float quadratic;

	PointLight(glm::vec3& position);
};