#pragma once

#include "../ecs/Transform.h"
#include "Octree.h"
#include "../render/Model.h"
#include "../render/Mesh.h"
#include "Triangle.h"
#include "Ray.h"
#include "../util/Time.h"

#include <vector>
#include <iostream>

class PhysicsSystem
{
public:

	Octree octree;

	PhysicsSystem();

	void SetPlayerCharacterTransform(Transform* playerTransform);
	void SetEnvironmentMesh(Model* environmentModel);

	void Update();

private:

	Model* environmentModel;
	std::vector<Triangle> triangles;
	
	Transform* playerTransform;

	void CalculatePlayerGravity();
};