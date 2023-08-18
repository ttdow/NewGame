#include "Octree.h"

Octree::Octree()
{
	this->root = OctreeNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

void Octree::Setup()
{
	float width = this->root.max.x - this->root.min.x;
	float height = this->root.max.y - this->root.min.y;
	float depth = this->root.max.z - this->root.min.z;
	
	//std::cout << "Bounding Volume Width: " << width << std::endl;
	//std::cout << "Bounding Volume Height: " << height << std::endl;
	//std::cout << "Bounding Volume Depth: " << depth << std::endl;

	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfDepth = depth / 2.0f;

	// Bottom front left.
	this->root.children.emplace_back(glm::vec3(this->root.min.x, this->root.min.y, this->root.min.z), 
									 glm::vec3(this->root.min.x + halfWidth, this->root.min.y + halfHeight, this->root.min.z + halfDepth));
	
	// Bottom front right.
	this->root.children.emplace_back(glm::vec3(this->root.min.x + halfWidth, this->root.min.y, this->root.min.z),
									 glm::vec3(this->root.max.x, this->root.min.y + halfHeight, this->root.min.z + halfDepth));

	// Top front left.
	this->root.children.emplace_back(glm::vec3(this->root.min.x, this->root.min.y + halfHeight, this->root.min.z),
									 glm::vec3(this->root.min.x + halfWidth, this->root.max.y, this->root.min.z + halfDepth));
	
	// Top front right.
	this->root.children.emplace_back(glm::vec3(this->root.min.x + halfWidth, this->root.min.y + halfHeight, this->root.min.z),
									 glm::vec3(this->root.max.x, this->root.max.y, this->root.min.z + halfDepth));
	
	// Bottom back left.
	this->root.children.emplace_back(glm::vec3(this->root.min.x, this->root.min.y, this->root.min.z + halfDepth),
									 glm::vec3(this->root.min.x + halfWidth, this->root.min.y + halfHeight, this->root.max.z));
	
	// Bottom back right.
	this->root.children.emplace_back(glm::vec3(this->root.min.x + halfWidth, this->root.min.y, this->root.min.z + halfDepth),
									 glm::vec3(this->root.max.x, this->root.min.y + halfHeight, this->root.max.z));
	
	// Top back left.
	this->root.children.emplace_back(glm::vec3(this->root.min.x, this->root.min.y + halfHeight, this->root.min.z + halfDepth),
									 glm::vec3(this->root.min.x + halfWidth, this->root.max.y, this->root.max.z));
	
	// Top back right.
	this->root.children.emplace_back(glm::vec3(this->root.min.x + halfWidth, this->root.min.y + halfHeight, this->root.min.z + halfDepth),
									 glm::vec3(this->root.max.x, this->root.max.y, this->root.max.z));
	
	//std::cout << "Children size: " << this->root.children.size() << std::endl;
}