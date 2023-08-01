#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Component.h"
#include "Entity.h"
#include "Transform.h"

#include <vector>
#include <string>

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