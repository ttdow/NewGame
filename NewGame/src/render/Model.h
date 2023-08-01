#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "mesh.h"
#include "shader.h"
#include "Bone.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
	
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::vector<Bone> bones;
	std::string directory;
	bool gammaCorrection;

	Model(std::string const& path, bool gamma = false);
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string filename, std::string directory, std::string textureType, bool gamma = false);

	void Draw(Shader& shader);
	void AttachTexture(std::string filename, std::string directory, std::string textureType);

private:

	void LoadModel(std::string const& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	Texture LoadTexture(std::string filename, std::string directory, std::string typeName);
};