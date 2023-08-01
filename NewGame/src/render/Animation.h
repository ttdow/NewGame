#pragma once

#include "Model.h"
#include "../util/Time.h"

#include <glm/glm.hpp>
#include <assimp/quaternion.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>

struct PositionKey
{

};

class Animation
{
public:

	glm::mat4 rootTransformation;
	glm::mat4 childTransformation;
	std::vector<glm::mat4> rotations;

	Animation(const std::string& animationPath, Model* model);
	unsigned int GetAnimationFrame();

private:

	float deltaTime;
	unsigned int currentFrame;

	glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);

};