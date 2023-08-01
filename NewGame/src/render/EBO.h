#pragma once

#include <glad/glad.h>

#include <vector>

class EBO
{
public:

	unsigned int id;

	EBO();
	void AddIndexData(std::vector<unsigned int>& indices);
};