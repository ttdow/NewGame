#include "Animator.h"

Animator::Animator(Animation* animation)
{
	this->PlayAnimation(animation);

	this->finalBoneMatrices.reserve(100);
	for (unsigned int i = 0; i < 100; i++)
	{
		this->finalBoneMatrices.push_back(glm::mat4(1.0f));
	}
}

void Animator::PlayAnimation(Animation* animation)
{
	this->currentTime = 0.0f;
	this->currentAnimation = animation;
}

void Animator::UpdateAnimation(float dt)
{
	this->deltaTime = dt;

	if (this->currentAnimation)
	{
		this->currentTime += this->currentAnimation->ticksPerSecond * dt;
		this->currentTime = fmod(this->currentTime, this->currentAnimation->duration);
		CalculateBoneTransform(&this->currentAnimation->rootNode, glm::mat4(1.0f));
	}
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* bone = this->currentAnimation->FindBone(nodeName);
	if (bone)
	{
		bone->Update(this->currentTime);
		nodeTransform = bone->localTransform;
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	std::map<std::string, BoneInfo> boneInfoMap = this->currentAnimation->boneInfoMap;
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		this->finalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
	{
		CalculateBoneTransform(&node->children[i], globalTransformation);
	}
}