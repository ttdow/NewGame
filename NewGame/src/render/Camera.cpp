#include "Camera.h"

Camera::Camera(Transform* playerTransform)
{
	this->playerTransform = playerTransform;

	this->position = *playerTransform->position;
	this->position.z -= 10.0f;
	this->position.y += glm::abs(this->position.z) * glm::tan(glm::radians(30.0f));

	this->front = glm::normalize(*this->playerTransform->position - this->position);

	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->worldUp = this->up;

	this->zoom = 45.0f;

	this->freeCamera = false;
	this->moveCamera = false;

	this->radialDistance = glm::sqrt((20.0f * 20.0f) + (glm::abs(this->position.z) * glm::tan(glm::radians(30.0f))) * (glm::abs(this->position.z) * glm::tan(glm::radians(30.0f))));
	this->inclination = 0.0f;
	this->azimuth = -180.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->position, *(this->playerTransform->position) + glm::vec3(0.0f, 2.0f, 0.0f), this->up);
}

void Camera::ProcessKeyboard()
{
	// Nothing
	return;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	if (this->freeCamera)
	{
		float deltaInclination = yoffset * 0.2f;
		this->inclination += deltaInclination;

		//float deltaAzimuth = xoffset * 0.33f;
		//this->azimuth -= deltaAzimuth;
	}

	if (this->moveCamera)
	{
		float deltaInclination = yoffset * 0.2f;
		this->inclination += deltaInclination;

		float deltaAzimuth = xoffset * 0.33f;
		this->azimuth -= deltaAzimuth;
	}
}

void Camera::ProcessMouseScroll(float yoffset)
{
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
	{
		zoom = 1.0f;
	}
	else if (zoom > 60.0f)
	{
		zoom = 60.0f;
	}
}

void Camera::Update()
{
	this->Update(0.0f, 0.0f, 0.0f);
}

void Camera::Update(float dr, float di, float da)
{
	this->radialDistance += dr;
	this->inclination += di;
	this->azimuth += da;

	// Clamp inclination angle.
	this->inclination = std::fmax(-90.0f, std::fmin(90.0f, this->inclination));

	// Clamp azimuth angle.
	this->azimuth = fmod(this->azimuth, 360.0f);

	// Update camera's position in Cartesian coords.
	float x = this->radialDistance * glm::sin(glm::radians(this->azimuth)) * glm::cos(glm::radians(this->inclination));
	float y = this->radialDistance * glm::sin(glm::radians(this->inclination));
	float z = this->radialDistance * glm::cos(glm::radians(this->azimuth)) * glm::cos(glm::radians(this->inclination));

	glm::vec3 playerPos = *this->playerTransform->position;

	this->position = glm::vec3(playerPos.x + x, playerPos.y + y, playerPos.z + z);
}