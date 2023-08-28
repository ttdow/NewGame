#include "ModelClass.h"

ModelClass::ModelClass(std::string filePath)
{
	this->filePath = filePath;
	Load();
}

void ModelClass::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		this->meshes[i].Draw(shader);
	}
}

void ModelClass::Load()
{
	std::cout << "Loading 3D model file with ASSIMP..." << std::endl;

	Time* timer = Time::GetInstance();
	float startTime = timer->GetTime();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(this->filePath,
		aiProcess_Triangulate |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals);// |
		//aiProcess_FlipUVs |
		//aiProcess_CalcTangentSpace);

	float endTime = timer->GetTime();
	std::cout << "Time for ASSIMP to parse model: " << endTime - startTime << std::endl;
	
	// Check for errors while loading the model.
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Retrieve the directory and filename from the filepath.
	this->dir = this->filePath.substr(0, this->filePath.find_last_of('/'));
	this->name = this->filePath.substr(this->filePath.find_last_of('/') + 1, this->filePath.size() - 1);

	// Get number of meshes in this model.
	this->numMeshes = scene->mNumMeshes;
	unsigned int numMaterials = scene->mNumMaterials;
	
	std::cout << "Loading textures for scene..." << std::endl;
	startTime = timer->GetTime();
	LoadMaterialTextures(scene);
	endTime = timer->GetTime();
	std::cout << "Time for engine to load textures: " << endTime - startTime << std::endl;

	// Process ASSIMP's root node recursively.
	startTime = timer->GetTime();
	ProcessNode(scene->mRootNode, scene);
	endTime = timer->GetTime();
	std::cout << "Time for engine to parse model: " << endTime - startTime << std::endl;
}

void ModelClass::Unload()
{
	delete(&this->meshes);
}

void ModelClass::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh contained in the current node.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// Get reference to current mesh from the scene.
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		// Process the current mesh and add it to a list of meshes for this model.
		ProcessMesh(mesh, scene);
	}

	// Recursively process the children nodes of this node.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void ModelClass::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Define data structures.
	this->meshes.emplace_back();
	MeshClass& myMesh = this->meshes.back();
	myMesh.name = mesh->mName.C_Str();

	// Iterate through each vertex in the mesh.
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		myMesh.vertices.emplace_back();
		Vertex& vertex = myMesh.vertices.back();
		SetVertexBoneDataToDefault(vertex);

		// Position data.
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;
		
		// Normal data.
		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		// Texture data.
		if (mesh->mTextureCoords[0] != nullptr)
		{
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
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
			myMesh.indices.emplace_back(face.mIndices[j]);
		}
	}

	if (mesh->HasBones())
	{
		// TODO
	}

	// Material data.
	aiString str;
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	std::string matName = mat->GetName().C_Str();

	// Save a reference to this mesh's material.
	for (unsigned int i = 0; i < this->materials.size(); i++)
	{
		if (std::strcmp(matName.c_str(), this->materials[i].name.c_str()) == 0)
		{
			myMesh.material = &this->materials[i];
		}
	}
	
	// Bind VAO, VBO, and EBO.
	myMesh.Setup();
}

void ModelClass::LoadMaterialTextures(const aiScene* scene)
{
	// Get a reference to the singleton texture manager.
	TextureManager* textureManager = TextureManager::GetInstance();

	// Loop through every material in the model.
	unsigned int numMaterials = scene->mNumMaterials;
	for (unsigned int i = 0; i < numMaterials; i++)
	{
		// Get the name of this material.
		aiString str;
		aiMaterial* mat = scene->mMaterials[i];
		std::string matName = mat->GetName().C_Str();

		// Ensure the material is not just an empty default material.
		if (std::strcmp(matName.c_str(), "DefaultMaterial") != 0)
		{
			// Ensure there is an albedo map for this material.
			if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				// Create a new material object for this material.
				this->materials.emplace_back(matName);

				// Get the albedo map name and save it to the material.
				mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
				this->materials.back().albedoMap = str.C_Str();

				// Create the albedo map texture and load it to the GPU.
				textureManager->AddTexture(this->materials.back().albedoMap);

				// Determine the filePath root from the albedo map.
				std::string root = this->materials.back().albedoMap.substr(0, this->materials.back().albedoMap.find_last_of('_') + 1);

				// Use the filePath root to find the metallic map.
				//this->materials.back().metallicMap = root + "Metalness.png";
				//textureManager->AddTexture(this->materials.back().metallicMap);

				// Use the filePath root to find the normal map.
				this->materials.back().normalMap = root + "Normal.png";
				textureManager->AddTexture(this->materials.back().normalMap);

				// Use the filePath root to find the roughness map.
				//this->materials.back().roughnessMap = root + "Roughness.png";
				//textureManager->AddTexture(this->materials.back().roughnessMap);
			}
		}
	}
}

// Initializes every bone ID to 999 and every weight to 0.0f
void ModelClass::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < 4; i++)
	{
		vertex.boneIds[i] = -1;
		vertex.weights[i] = 0.0f;
	}
}