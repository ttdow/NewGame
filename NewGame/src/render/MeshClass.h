#pragma once

#include "Vertex.h"
#include "Material.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "TextureManager.h"

#include <vector>
#include <iostream>

class MeshClass
{
public:

	std::string name;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Material* material;

	MeshClass();
	void Setup();
	void Draw(Shader& shader);

private:

	VAO* vao;
	VBO* vbo;
	EBO* ebo;

};