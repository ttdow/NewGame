#pragma once

#include "Vertex.h"

#include <vector>

class CubeVertices
{
private:

	static CubeVertices* instance;

	CubeVertices();

	CubeVertices(const CubeVertices&) = delete;
	CubeVertices& operator=(const CubeVertices&) = delete;

public:

	static CubeVertices* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new CubeVertices();
		}

		return instance;
	}

	static std::vector<Vertex> GetVertices();
	static std::vector<unsigned int> GetIndices();
};