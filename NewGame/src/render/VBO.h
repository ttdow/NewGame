#pragma once

#include <glad/glad.h>
//#include <glm/glm.hpp>

#include "Vertex.h"

#include <vector>
#include <iostream>

class VBO
{
private:

public:

	unsigned int id;

	VBO();
	void AddVertexData(std::vector<Vertex>& vertices);
	//void AddPositionData(float positions[]);
	//void AddNormalData();
	//void AddTextureData();
};