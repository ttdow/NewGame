#pragma once

#include "Shader.h"
#include "Texture.h"

#include <string>

class Material
{
public:

	TextureClass* albedo;
	TextureClass* normal;
	TextureClass* metallic;
	TextureClass* roughness;
	TextureClass* ao;

	Material(std::string dir);
	void BindMaterial();
};