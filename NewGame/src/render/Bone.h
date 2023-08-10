#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/matrix.hpp>
#include <assimp/scene.h>

#include <vector>
#include <string>

struct KeyPosition
{
	glm::vec3 position;
	float timestamp;
};

struct KeyRotation
{
	glm::quat rotation;
	float timestamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timestamp;
};

class Bone
{
public:

	glm::mat4 localTransform;
	std::string name;
	int id;

	Bone(const std::string& name, int id, const aiNodeAnim* channel);
	void Update(float animationTime);

	int GetPositionIndex(float animationTime);
	int GetRotationIndex(float animationTime);
	int GetScaleIndex(float animationTime);

	float GetScaleFactor(float lastTimeStep, float nextTimeStep, float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);
	glm::mat4 InterpolateRotation(float animationTime);
	glm::mat4 InterpolateScale(float animationTime);

private:

	std::vector<KeyPosition> positions;
	std::vector<KeyRotation> rotations;
	std::vector<KeyScale> scales;

	int numPositions;
	int numRotations;
	int numScales;
};