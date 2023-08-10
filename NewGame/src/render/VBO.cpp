#include "VBO.h"

VBO::VBO()
{
	// Create a new buffer.
	glGenBuffers(1, &id);
}

void VBO::AddVertexData(std::vector<Vertex>& vertices)
{
	// Bind this buffer.
	glBindBuffer(GL_ARRAY_BUFFER, id);

	// Determine size of vertex data.
	unsigned int size = vertices.size() * sizeof(Vertex);

	// Add vertex data to buffer.
	glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW);

	// Unbind this buffer.
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}