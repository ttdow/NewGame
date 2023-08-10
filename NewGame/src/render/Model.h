#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "Mesh.h"
#include "Shader.h"
#include "Bone.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

struct BoneInfo
{
	int id;
	glm::mat4 offset;
};

class Model
{
public:
	
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	std::map<std::string, BoneInfo> boneInfoMap;
	int boneCounter;

	Model(std::string const& path, bool gamma = false);
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string filename, std::string directory, std::string textureType, bool gamma = false);

	void Draw(Shader& shader);
	void AttachTexture(std::string filename, std::string directory, std::string textureType);

private:

	void SetVertexBoneDataToDefault(Vertex& vertex);
	void SetVertexBoneData(Vertex& vertex, int boneId, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	void LoadModel(std::string const& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	Texture LoadTexture(std::string filename, std::string directory, std::string typeName);

	glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);
};