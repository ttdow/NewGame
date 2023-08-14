#pragma once

#include <glad/glad.h>

#include "stb_image.h"

#include <string>
#include <iostream>

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