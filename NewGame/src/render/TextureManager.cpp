#include "TextureManager.h"

TextureManager* TextureManager::instance = nullptr;

void TextureManager::AddTexture(std::string fileName)
{
	// Create a new texture.
	Texture* newTexture = new Texture();

	// Insert the new texture into the unordered map.
	auto ret = this->textures.emplace(std::pair<std::string, Texture*>(fileName, newTexture));
	
	// Delete the new texture if the insertion failed (i.e. it already exists).
	if (!ret.second)
	{
		delete(newTexture);
		return;
	}

	// Load texture data if this is a new texture.
	newTexture->Load("res/obj/sponza/" + fileName);
}

unsigned int TextureManager::GetTextureId(std::string fileName)
{
	// Find the texture if it exists and return the textureId.
	auto it = this->textures.find(fileName);
	if (it != this->textures.end())
	{
		return (*it).second->id;
	}
	else
	{
		return 99;
	}
}