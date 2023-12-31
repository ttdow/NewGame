#include "Model.h"

Model::Model(std::string const& path, bool gamma)
{
	LoadModel(path);
}

Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string filename, std::string directory, std::string textureType, bool gamma)
{
	this->gammaCorrection = gamma;

	this->textures_loaded.push_back(TextureClass(directory + filename, false));
	this->textures_loaded[0].SetType(textureType);

	this->textures_loaded.push_back(TextureClass("res/textures/brickwall_normal.jpg", false));
	this->textures_loaded[1].SetType("texture_normal");

	this->meshes.push_back(Mesh(vertices, indices, textures_loaded, false));

	this->boneCounter = 0;
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::LoadModel(std::string const& path)
{
	// Read file via ASSIMP.
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	
	// Check for errors while loading the model.
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Retrieve the directory path of the filepath.
	directory = path.substr(0, path.find_last_of('/'));

	// Process ASSIMP's root node recursively.
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh contained in the current node.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// Get reference to current mesh from the scene.
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		// Process the current mesh and add it to a list of meshes for this model.
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// Recursively process the children nodes of this node.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Define data structures.
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureClass> textures;

	// Iterate through each vertex in the mesh.
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		SetVertexBoneDataToDefault(vertex);

		glm::vec3 vector;

		// Position data.
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		// Normal data.
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		// Texture data.
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;

			// TODO a vertex can contain up to 8 different texture coords.
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			//vertex.texCoords = vec;
		}
		else
		{
			//vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		// Add the new vertex to the list.
		vertices.push_back(vertex);
	}
	
	// Iterate through each face in the mesh and get the indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		// Get a reference to the current face.
		aiFace face = mesh->mFaces[i];

		// Get each vertex in the face.
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			// Add each index to a list.
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process materials.
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// Load textures.
	// 1. Diffuse maps.
	std::vector<TextureClass> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// 2. Specular maps.
	std::vector<TextureClass> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3. Normal maps.
	std::vector<TextureClass> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// 4. Height maps.
	std::vector<TextureClass> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	if (textures.size() < 1)
	{
		TextureClass diffuseTex = TextureClass("res/textures/gobbo.png", false);
		diffuseTex.SetType("texture_diffuse");
		textures.insert(textures.end(), diffuseTex);
	}

	bool animated = false;

	// Get animation data.
	if (mesh->HasBones())
	{
		ExtractBoneWeightForVertices(vertices, mesh, scene);
		animated = true;
	}

	// Return the new mesh.
	return Mesh(vertices, indices, textures, animated);
}

std::vector<TextureClass> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	// Define data structure.
	std::vector<TextureClass> textures;

	// Iterate through each texture in the material.
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		// Determine texture type (i.e. diffuse, specular, normal, height).
		aiString str;
		mat->GetTexture(type, i, &str);

		// Do not create a new texture if the current texture already exists.
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].GetFilePath().data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		// Create a new texture if the current texture does not already exist.
		if (!skip)
		{
			std::string filePath = this->directory + "/" + std::string(str.C_Str());

			TextureClass texture = TextureClass(filePath, false);
			texture.SetType(typeName);
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

// Initializes every bone ID to 999 and every weight to 0.0f
void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < 4; i++)
	{
		vertex.boneIds[i] = -1;
		vertex.weights[i] = 0.0f;
	}
}

void Model::SetVertexBoneData(Vertex& vertex, int boneId, float weight)
{
	for (int i = 0; i < 4; i++)
	{
		if (vertex.boneIds[i] < 0)
		{
			vertex.weights[i] = weight;
			vertex.boneIds[i] = boneId;
			break;
		}
	}
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
	{
		int boneId = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();

		if (this->boneInfoMap.find(boneName) == this->boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = this->boneCounter;
			newBoneInfo.offset = ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);

			this->boneInfoMap[boneName] = newBoneInfo;
			boneId = this->boneCounter;
			this->boneCounter++;
		}
		else
		{
			boneId = this->boneInfoMap[boneName].id;
		}

		assert(boneId != -1);

		aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; weightIndex++)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			
			assert(vertexId <= vertices.size());

			SetVertexBoneData(vertices[vertexId], boneId, weight);
		}
	}
}

glm::mat4 Model::ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
{
	glm::mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}