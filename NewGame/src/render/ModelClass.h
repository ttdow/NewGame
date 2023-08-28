#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "MeshClass.h"
#include "Shader.h"
#include "Material.h"
#include "../util/Time.h"
#include "TextureManager.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

class ModelClass
{
public:

	std::string filePath;
	std::string dir;
	std::string name;

	ModelClass(std::string filePath);
	void Draw(Shader& shader);
	void Load();
	void Unload();

private:

	unsigned int numMeshes;

	std::vector<Material> materials;
	std::vector<MeshClass> meshes;

	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void SetVertexBoneDataToDefault(Vertex& vertex);
	void LoadMaterialTextures(const aiScene* scene);
};