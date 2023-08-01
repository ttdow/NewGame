#include "Entity.h"

Entity::Entity(std::string name)
{
	this->name = name;
}

void Entity::SetChild(Entity* child)
{
	this->children.emplace_back(child);
	this->children.back()->parent = this;
}

Transform* Entity::GetTransform()
{
	for (Component* component : components)
	{
		Transform* ptr = dynamic_cast<Transform*>(component);
		if (ptr)
		{
			return ptr;
		}
	}

	return nullptr;
}

MeshRenderer* Entity::GetRenderer()
{
	for (Component* component : components)
	{
		MeshRenderer* ptr = dynamic_cast<MeshRenderer*>(component);
		if (ptr)
		{
			return ptr;
		}
	}

	return nullptr;
}