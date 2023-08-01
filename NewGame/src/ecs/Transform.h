#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

#include <iostream>

class Entity;

class Transform : public Component
{
public:

	glm::vec3* position;
	glm::vec3* rotation;
	glm::vec3* scale;

	glm::mat4 modelMatrix;

private:

	glm::quat* qRotation;

	void UpdateQRotation();
	glm::mat4 CalculateMatrix();

public:

	Transform(Entity* entity, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	~Transform();

	void Update();
	void Update(glm::vec3 position, glm::vec3 rotation);
	void Update(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

};