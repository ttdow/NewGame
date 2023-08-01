#pragma once

#include "Component.h"
#include "Transform.h"
#include "../render/MeshRenderer.h"

#include <string>
#include <vector>
#include <iostream>
#include <list>

class Entity
{
public:

	std::string name;
	std::vector<Component*> components;
	std::list<Entity*> children;

	Entity* parent;

	Entity(std::string name);
	
	void SetChild(Entity* child);

	Transform* GetTransform();
	MeshRenderer* GetRenderer();
};