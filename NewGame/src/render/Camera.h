#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../ecs/Transform.h"

#include <vector>
#include <iostream>

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 250.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:

	glm::vec3 position;

	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 worldUp;

	float radialDistance;
	float inclination;
	float azimuth;

	float zoom;

	bool freeCamera;
	bool moveCamera;

	Camera(Transform* playerTransform);

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard();
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);
	void Update();
	void Update(float dr, float di, float da);

private:

	Transform* playerTransform;
};