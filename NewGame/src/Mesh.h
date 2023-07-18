#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	//glm::vec3 color;
	glm::vec2 texCoords;
	//glm::vec3 tangent;
	//glm::vec3 biTangent;

	//int m_BoneIDs[MAX_BONE_INFLUENCE];
	//float m_Weights[MAX_BONE_INFLUENCE];
};

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
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void Draw(Shader& shader);

private:

	unsigned int VBO, EBO;

	void SetupMesh();
};