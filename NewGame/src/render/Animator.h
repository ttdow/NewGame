#pragma once

#include <glm/glm.hpp>

#include "Animation.h"

#include <vector>
#include <string>

class Animator
{
public:

	std::vector<glm::mat4> finalBoneMatrices;
	Animation* currentAnimation;
	float currentTime;
	float deltaTime;

	Animator(Animation* animation);
	void PlayAnimation(Animation* animation);
	void UpdateAnimation(float dt);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
};