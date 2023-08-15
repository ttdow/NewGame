#pragma once

#include "Shader.h"
#include "Texture.h"

class Material
{
public:

	Shader* shader;

	TextureClass* albedo;
	TextureClass* normal;
	TextureClass* metallic;
	TextureClass* roughness;
	TextureClass* ao;

	void SetMaterial();

};