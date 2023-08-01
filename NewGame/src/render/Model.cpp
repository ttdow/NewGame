#include "Model.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nChannels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nChannels, 0);
	if (data)
	{
		GLenum format;
		if (nChannels == 1) format = GL_RED;
		else if (nChannels == 3) format = GL_RGB;
		else if (nChannels == 4) format = GL_RGBA;
		else format = GL_RGB;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}

Model::Model(std::string const& path, bool gamma)
{
	LoadModel(path);
}

Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string filename, std::string directory, std::string textureType, bool gamma)
{
	this->gammaCorrection = gamma;

	this->textures_loaded.push_back(LoadTexture(filename, directory, textureType));
	
	stbi_set_flip_vertically_on_load(false);
	this->textures_loaded.push_back(LoadTexture("brickwall_normal.jpg", "res/textures/", "texture_normal"));
	stbi_set_flip_vertically_on_load(true);

	this->bones = std::vector<Bone>();

	this->meshes.push_back(Mesh(vertices, indices, textures_loaded, bones));
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
	std::vector<Texture> textures;

	unsigned int counter = 0;

	// Iterate through each vertex in the mesh.
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
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
			vertex.texCoords = vec;

			/*
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			*/
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		
		if (mesh->HasBones())
		{
			// TODO Print this vertex's bone information for debugging.
			/*
			std::cout << "NumBones = " << mesh->mNumBones << std::endl;
			for (unsigned int j = 0; j < mesh->mNumBones; j++)
			{
				std::cout << "Bone: " << mesh->mBones[j]->mName.data;
				std::cout << " - VertexID: " << mesh->mBones[j]->mWeights[i].mVertexId;
				std::cout << ", Weight: " << mesh->mBones[j]->mWeights[i].mWeight;
				std::cout << std::endl;
			}
			*/

			Bone newBone;
			for (unsigned int j = 0; j < 4; j++)
			{
				if (j < mesh->mNumBones)
				{
					newBone.boneId[j] = mesh->mBones[j]->mWeights[i].mVertexId;
					newBone.weight[j] = mesh->mBones[j]->mWeights[i].mWeight;
				}
				else
				{
					newBone.boneId[j] = 999;
					newBone.weight[j] = -1.0f;
				}
			}

			this->bones.push_back(newBone);
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

	// TODO Print vertex bone data for debugging.
	/*
	for (unsigned int i = 0; i < this->bones.size(); i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			std::cout << "Vertex: " << i;
			std::cout << ", Index: " << indices[i] << " - ";
			std::cout << this->bones[i].boneId[j] << ": ";
			std::cout << this->bones[i].weight[j];
			std::cout << std::endl;
		}
	}
	*/

	// Process materials.
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// Load textures.
	// 1. Diffuse maps.
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	// 2. Specular maps.
	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	// 3. Normal maps.
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	// 4. Height maps.
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	if (textures.size() < 1)
	{
		Texture diffuseMap = LoadTexture("container.jpg", "res/textures/", "texture_diffuse");
		textures.insert(textures.end(), diffuseMap);
	}

	// TODO 56 vertices seems like too many.
	/*
	if (vertices.size() == 56)
	{
		std::vector<Vertex> vertices2;
		vertices2.insert(vertices2.end(), vertices.begin(), vertices.begin() + 16);
		std::cout << vertices2.size() << std::endl;
		
		for (unsigned int i = 0; i < indices.size(); i++)
		{
			std::cout << indices[i] << std::endl;
		}

		return Mesh(vertices2, indices, textures);
	}
	*/

	// Return the new mesh.
	return Mesh(vertices, indices, textures, bones);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	// Define data structure.
	std::vector<Texture> textures;

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
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		// Create a new texture if the current texture does not already exist.
		if (!skip)
		{
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

Texture Model::LoadTexture(std::string filename, std::string directory, std::string typeName)
{
	Texture texture;
	texture.ID = TextureFromFile(filename.c_str(), directory);
	texture.type = typeName;
	texture.path = filename.c_str();
	
	return texture;
}

void Model::AttachTexture(std::string filename, std::string directory, std::string typeName)
{

}