#include "EBO.h"

EBO::EBO()
{
	glGenBuffers(1, &id);
}

void EBO::AddIndexData(std::vector<unsigned int>& indices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}