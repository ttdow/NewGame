#pragma once

#include "Texture.h"

#include <string>
#include <unordered_map>

class TextureManager
{
public:

	std::unordered_map<std::string, Texture*> textures;

	static TextureManager* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new TextureManager();
		}

		return instance;
	}

	void AddTexture(std::string fileName);
	unsigned int GetTextureId(std::string fileName);

private:

	static TextureManager* instance;

	TextureManager() {}
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};