#include "Transform.h"

#include "../ecs/Entity.h"

Transform::Transform(Entity* entity, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->entity = entity;

	this->position = new glm::vec3(position);
	this->rotation = new glm::vec3(rotation);
	this->scale = new glm::vec3(scale);
	this->qRotation = new glm::quat(rotation);

	this->modelMatrix = CalculateMatrix();
}

Transform::~Transform()
{
	delete(this->position);
	delete(this->rotation);
	delete(this->scale);
	delete(this->qRotation);
}

void Transform::Update()
{
	// Update local -> world (model) matrix.
	this->modelMatrix = CalculateMatrix();
}

void Transform::Update(glm::vec3 position, glm::vec3 rotation)
{
	// Update position information.
	this->position->x = position.x;
	this->position->y = position.y;
	this->position->z = position.z;

	// Update rotation information.
	this->rotation->x = rotation.x;
	this->rotation->y = rotation.y;
	this->rotation->z = rotation.z;

	// Update local -> world (model) matrix.
	this->modelMatrix = CalculateMatrix();
}

void Transform::Update(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	// Update scale information.
	this->scale->x = scale.x;
	this->scale->y = scale.y;
	this->scale->z = scale.z;

	// Update position and rotation information.
	Update(position, rotation);
}

void Transform::UpdateQRotation()
{
	// Create a quaternion rotation using the Euler rotation.
	glm::quat tempQuat = glm::quat(*this->rotation);

	// Update quaternion rotation data.
	this->qRotation->x = tempQuat.x;
	this->qRotation->y = tempQuat.y;
	this->qRotation->z = tempQuat.z;
	this->qRotation->w = tempQuat.w;
}

glm::mat4 Transform::CalculateMatrix()
{
	// Create translation matrix.
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), *this->position);

	// Create rotation matrix.
	UpdateQRotation();
	glm::mat4 rotate = glm::toMat4(*this->qRotation);

	// Create scaling matrix.
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), *this->scale);

	// Multiply matrices to create model matrix.
	glm::mat4 model = translate * rotate * scale * glm::mat4(1.0f);

	// Update hierarchy.
	if (this->entity->parent)
	{
		// Multiply this entity's model matrix by their parent's.
		model = this->entity->parent->GetTransform()->modelMatrix * model;

		// Update this entity's children with the new model matrix.
		for (Entity* child : this->entity->children)
		{
			child->GetTransform()->Update();
		}
	}

	return model;
}