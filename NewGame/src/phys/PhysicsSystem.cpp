#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
	this->octree = Octree();
}

void PhysicsSystem::SetPlayerCharacterTransform(Transform* playerTransform)
{
	this->playerTransform = playerTransform;
}

void PhysicsSystem::SetEnvironmentMesh(Model* environmentModel)
{
	// Load the triangular mesh from the environment model for physics calculations.
	this->environmentModel = environmentModel;

	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();;
	float maxZ = std::numeric_limits<float>::min();;

	for (unsigned int j = 0; j < this->environmentModel->meshes.size(); j++)
	{
		Mesh& mesh = this->environmentModel->meshes[j];

		for (unsigned int i = 0; i < mesh.indices.size(); i += 3)
		{
			Triangle newTriangle;

			newTriangle.i0 = mesh.indices[i];
			newTriangle.i1 = mesh.indices[i + 1];
			newTriangle.i2 = mesh.indices[i + 2];

			newTriangle.v0 = mesh.vertices[newTriangle.i0].position;
			newTriangle.v1 = mesh.vertices[newTriangle.i1].position;
			newTriangle.v2 = mesh.vertices[newTriangle.i2].position;

			newTriangle.e0 = newTriangle.v1 - newTriangle.v0;
			newTriangle.e1 = newTriangle.v2 - newTriangle.v0;

			newTriangle.normal = glm::normalize(glm::cross(newTriangle.e0, newTriangle.e1));

			this->triangles.push_back(newTriangle);

			// Get bounding volume data for entire terrain.
			minX = glm::min(minX, newTriangle.v0.x);
			minX = glm::min(minX, newTriangle.v1.x);
			minX = glm::min(minX, newTriangle.v2.x);

			minY = glm::min(minY, newTriangle.v0.y);
			minY = glm::min(minY, newTriangle.v1.y);
			minY = glm::min(minY, newTriangle.v2.y);

			minZ = glm::min(minZ, newTriangle.v0.z);
			minZ = glm::min(minZ, newTriangle.v1.z);
			minZ = glm::min(minZ, newTriangle.v2.z);

			maxX = glm::max(maxX, newTriangle.v0.x);
			maxX = glm::max(maxX, newTriangle.v1.x);
			maxX = glm::max(maxX, newTriangle.v2.x);

			maxY = glm::max(maxY, newTriangle.v0.y);
			maxY = glm::max(maxY, newTriangle.v1.y);
			maxY = glm::max(maxY, newTriangle.v2.y);

			maxZ = glm::max(maxZ, newTriangle.v0.z);
			maxZ = glm::max(maxZ, newTriangle.v1.z);
			maxZ = glm::max(maxZ, newTriangle.v2.z);
		}
	}

	// Save bounding box volume to root node of Octree.
	glm::vec3 min = glm::vec3(minX, minY, minZ);
	glm::vec3 max = glm::vec3(maxX, maxY, maxZ);
	this->octree.root.min = min;
	this->octree.root.max = max;

	this->octree.Setup();
}

void PhysicsSystem::Update()
{
	CalculatePlayerGravity();
}

void PhysicsSystem::CalculatePlayerGravity()
{
	// Define ray originating at the player's position and shooting downward along gravity vector.
	Ray playerRay(*this->playerTransform->position, glm::vec3(0.0f, -1.0f, 0.0f));

	int collisionCounter = 0;

	// Search the terrain mesh for collision points.
	std::vector<glm::vec3> collisionPoints;
	glm::vec3 collisionPoint = *this->playerTransform->position;
	for (unsigned int i = 0; i < this->triangles.size(); i++)
	{
		// Check if ray and triangle normal are parallel.
		float dotProduct = glm::dot(playerRay.GetDirection(), this->triangles[i].normal);
		if (glm::abs(dotProduct) <= 0.1f)
		{
			// Discard triangles whose normal is parallel to the ray.
			continue;
		}

		// Test to see if the ray intersects the triangle's plane.
		// t = - (N . (O - Po)) / (N . D)
		float numerator = glm::dot(this->triangles[i].normal, (playerRay.origin - this->triangles[i].v0));
		float denominator = glm::dot(this->triangles[i].normal, playerRay.GetDirection());

		float t = -1.0f * (numerator / denominator);

		// Discard triangles who are behind the ray's origin.
		if (t < 0)
		{
			//continue;
		}

		// Determine the intersection point P.
		glm::vec3 P = playerRay.origin + (t * playerRay.GetDirection());

		if (isnan(P.x) || isnan(P.y) || isnan(P.z))
		{
			continue;
		}

		// Calculate the vectors from each vertex of the triangles to P.
		glm::vec3 v0P = P - this->triangles[i].v0;
		glm::vec3 v1P = P - this->triangles[i].v1;
		glm::vec3 v2P = P - this->triangles[i].v2;

		// Calculate the areas of the sub-triangles formed by P and the edges of the triangle.
		float a0 = 0.5f * glm::length(glm::cross(v1P, v2P));
		float a1 = 0.5f * glm::length(glm::cross(v2P, v0P));
		float a2 = 0.5f * glm::length(glm::cross(v0P, v1P));

		// Calculate total area of triangle.
		float totalArea = 0.5f * glm::length(glm::cross((this->triangles[i].v1 - this->triangles[i].v0), (this->triangles[i].v2 - this->triangles[i].v0)));

		// Calculate the baycentric coords.
		float u = a0 / totalArea;
		float v = a1 / totalArea;
		float w = a2 / totalArea;
		float sum = u + v + w;

		// Discard if the intersection is outside of the area.
		if (sum <= 0.98f || sum >= 1.02f) // Error margin for floating point calculations.
		{
			continue;
		}

		if (u < 0 || u > 1)
		{
			continue;
		}
		else if (v < 0 || v > 1)
		{
			continue;
		}
		else if (w < 0 || w > 1)
		{
			continue;
		}

		collisionPoint = P;
		collisionCounter++;

		collisionPoints.push_back(collisionPoint);

		//std::cout << "Collision Triangle v0: (" << this->triangles[i].v0.x << ", " << this->triangles[i].v0.y << ", " << this->triangles[i].v0.z << ")" << std::endl;
		//std::cout << "Collision Triangle v1: (" << this->triangles[i].v1.x << ", " << this->triangles[i].v1.y << ", " << this->triangles[i].v1.z << ")" << std::endl;
		//std::cout << "Collision Triangle v2: (" << this->triangles[i].v2.x << ", " << this->triangles[i].v2.y << ", " << this->triangles[i].v2.z << ")" << std::endl;
		//std::cout << std::endl;
	}

	// Find the highest collision point underneath the player.
	glm::vec3 playerPos = *this->playerTransform->position;
	glm::vec3 playerCenter = playerPos + glm::vec3(0.0f, 5.0f, 0.0f);
	float highestYUnderPlayer = std::numeric_limits<float>::min();
	unsigned int correctCollisionPoint = 999;
	for (unsigned int i = 0; i < collisionPoints.size(); i++)
	{
		if (collisionPoints[i].y <= playerCenter.y)
		{
			if (collisionPoints[i].y >= highestYUnderPlayer)
			{
				highestYUnderPlayer = collisionPoints[i].y;
				correctCollisionPoint = i;
			}
		}
	}

	if (correctCollisionPoint != 999)
	{
		collisionPoint = collisionPoints[correctCollisionPoint];
	}

	//std::cout << "Collision Counter: " << collisionCounter << std::endl;
	//std::cout << "Player position: (" << playerPos.x << ", " << playerPos.y << ", " << playerPos.z << ")" << std::endl;
	//std::cout << "Collision position: (" << collisionPoint.x << ", " << collisionPoint.y << ", " << collisionPoint.z << ")" << std::endl;

	// Smooth falling time.
	Time* timer = Time::GetInstance();
	float heightDiff = this->playerTransform->position->y - collisionPoint.y;
	if (glm::abs(heightDiff) <= 2.0f)
	{
		// Update the player's transform using the terrain height.
		this->playerTransform->position->y = collisionPoint.y;
	}
	else
	{
		if (heightDiff > 0)
		{
			this->playerTransform->position->y -= 2.0f * timer->GetDeltaTime() * 10.0f;
		}
		else
		{
			this->playerTransform->position->y += 2.0f * timer->GetDeltaTime() * 10.0f;
		}
	}
	
}