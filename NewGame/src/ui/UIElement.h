#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../render/Shader.h"

#include <iostream>
#include <map>
#include <string>

struct Character
{
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class UIElement
{
public:

	UIElement();
	void RenderText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color);

	std::map<char, Character> characters;
	unsigned int VAO, VBO;
};