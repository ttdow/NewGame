#pragma once

#include "Shader.h"
#include "Texture.h"

#include <string>

class Material
{
public:

	std::string name;
	//std::string filePath;
	unsigned int id;

	std::string albedoMap;
	std::string normalMap;
	std::string metallicMap;
	std::string roughnessMap;
	//std::string aoMap;

	Material(std::string name);
	void BindMaterial();
};