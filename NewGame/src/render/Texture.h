#pragma once

#include <glad/glad.h>

#include "stb_image.h"

#include <string>
#include <iostream>

class Texture
{
public:

	unsigned int id;
	std::string filePath;

	void Load(std::string filePath);

private:

	std::string type;

};

class TextureClass
{
public:

	TextureClass(std::string filePath, bool flipped);
	unsigned int GetID();
	std::string GetFilePath();
	std::string GetType();
	void SetType(std::string type);
	void SetWrapToClamped();

private:

	unsigned int id;
	std::string filePath;
	std::string type;
};