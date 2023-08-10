#pragma once

#include <glad/glad.h>

#include "Vertex.h"
#include "Bone.h"

#include <vector>
#include <iostream>

class VBO
{
private:

public:

	unsigned int id;

	VBO();
	void AddVertexData(std::vector<Vertex>& vertices);
};