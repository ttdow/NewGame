#pragma once

#include <glm/glm.hpp>
#include <assimp/quaternion.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../util/Time.h"
#include "Model.h"

#include <string>
#include <iostream>
#include <map>

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:

	float duration;
	float ticksPerSecond;
	AssimpNodeData rootNode;
	std::map<std::string, BoneInfo> boneInfoMap;

	Animation() = default;
	Animation(const std::string& animationPath, Model* model);
	~Animation() {}
	Bone* FindBone(const std::string& name);

	static glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);

private:

	float deltaTime;
	unsigned int currentFrame;

	std::vector<Bone> bones;

	void ReadMissingBones(const aiAnimation* animation, Model& model);
	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
};