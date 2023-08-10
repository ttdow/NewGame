#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

#include <string>
#include <vector>

struct Texture
{
	unsigned int ID;
	std::string type;
	std::string path;
};

class Mesh
{
public:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<Bone> bones;

	VAO* vao;
	VBO* vbo;
	EBO* ebo;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, bool animated);

	void Draw(Shader& shader);

private:

	bool animated;

	void SetupMesh();
};