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

#define MAX_BONE_INFLUENCE 4

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

	unsigned int vao;

	VAO* vaoTest;
	VBO* vboTest;
	EBO* eboTest;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<Bone> bones);

	void Draw(Shader& shader);

private:

	unsigned int vbo, ebo;

	void SetupMesh();
};