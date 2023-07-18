#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <string>

class Component
{
public:

	virtual ~Component() {}
};

struct Transform : public Component
{
public:

	glm::vec3* position;
	glm::vec3* rotation;
	glm::vec3* scale;

private:

	glm::quat* qRotation;

public:

	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	{
		this->position = new glm::vec3(position);
		this->rotation = new glm::vec3(rotation);
		this->scale = new glm::vec3(scale);
		this->qRotation = new glm::quat(rotation);
	}

	~Transform()
	{
		delete(this->position);
		delete(this->rotation);
		delete(this->scale);
		delete(this->qRotation);
	}
};

struct Entity
{
	std::string name;
	std::vector<Component*> components;

	Entity(std::string name)
	{
		this->name = name;
	}

	~Entity()
	{
		for (auto component : components)
		{
			delete component;
		}
	}
};

class ComponentManager
{
private:

	std::vector<Entity*> entities;

public:

	Entity& CreateEntity(std::string name)
	{
		Entity* entity = new Entity(name);
		entities.push_back(entity);
		
		return *entity;
	}

	Transform& AddTransform(Entity& entity)
	{
		// Default values.
		glm::vec3 position(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation(0.0f, 0.0f, 0.0f);
		glm::vec3 scale(1.0f, 1.0f, 1.0f);

		Transform* transform = new Transform(position, rotation, scale);
		entity.components.push_back(transform);
		return *transform;
	}
};