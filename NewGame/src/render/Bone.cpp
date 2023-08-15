#include "Bone.h"

Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel)
{
	this->name = name;
	this->id = id;
	this->localTransform = glm::mat4(1.0f);

	this->numPositions = channel->mNumPositionKeys;
	for (int positionIndex = 0; positionIndex < this->numPositions; positionIndex++)
	{
		glm::vec3 position;
		position.x = channel->mPositionKeys[positionIndex].mValue.x;
		position.y = channel->mPositionKeys[positionIndex].mValue.y;
		position.z = channel->mPositionKeys[positionIndex].mValue.z;
		float timestamp = channel->mPositionKeys[positionIndex].mTime;

		KeyPosition data;
		data.position = position;
		data.timestamp = timestamp;
		this->positions.push_back(data);
	}

	this->numRotations = channel->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < this->numRotations; rotationIndex++)
	{
		glm::quat rotation;
		rotation.x = channel->mRotationKeys[rotationIndex].mValue.x;
		rotation.y = channel->mRotationKeys[rotationIndex].mValue.y;
		rotation.z = channel->mRotationKeys[rotationIndex].mValue.z;
		rotation.w = channel->mRotationKeys[rotationIndex].mValue.w;
		float timestamp = channel->mRotationKeys[rotationIndex].mTime;

		KeyRotation data;
		data.rotation = rotation;
		data.timestamp = timestamp;
		this->rotations.push_back(data);
	}

	this->numScales = channel->mNumScalingKeys;
	for (int scaleIndex = 0; scaleIndex < this->numScales; scaleIndex++)
	{
		glm::vec3 scale;
		scale.x = channel->mScalingKeys[scaleIndex].mValue.x;
		scale.y = channel->mScalingKeys[scaleIndex].mValue.y;
		scale.z = channel->mScalingKeys[scaleIndex].mValue.z;
		float timestamp = channel->mScalingKeys[scaleIndex].mTime;

		KeyScale data;
		data.scale = scale;
		data.timestamp = timestamp;
		this->scales.push_back(data);
	}
}

void Bone::Update(float animationTime)
{
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScale(animationTime);

	this->localTransform = translation * rotation * scale;
}

int Bone::GetPositionIndex(float animationTime)
{
	for (int i = 0; i < this->numPositions - 1; i++)
	{
		if (animationTime < this->positions[i + 1].timestamp)
		{
			return i;
		}
	}

	return 0;
	assert(0);
}

int Bone::GetRotationIndex(float animationTime)
{
	for (int i = 0; i < this->numRotations - 1; i++)
	{
		if (animationTime < this->rotations[i + 1].timestamp)
		{
			return i;
		}
	}

	return 0;
	assert(0);
}

int Bone::GetScaleIndex(float animationTime)
{
	for (int i = 0; i < this->numScales - 1; i++)
	{
		if (animationTime < this->scales[i + 1].timestamp)
		{
			return i;
		}
	}

	return 0;
	assert(0);
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	if (this->numPositions == 1)
	{
		return glm::translate(glm::mat4(1.0f), this->positions[0].position);
	}

	int p0Index = GetPositionIndex(animationTime);
	int p1Index = p0Index + 1;

	float scaleFactor = GetScaleFactor(this->positions[p0Index].timestamp, this->positions[p1Index].timestamp, animationTime);

	glm::vec3 finalPosition = glm::mix(this->positions[p0Index].position, this->positions[p1Index].position, scaleFactor);

	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	if (this->numRotations == 1)
	{
		glm::quat rotation = glm::normalize(this->rotations[0].rotation);
		return glm::toMat4(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	int p1Index = p0Index + 1;

	float scaleFactor = GetScaleFactor(this->rotations[p0Index].timestamp, this->rotations[p1Index].timestamp, animationTime);

	glm::quat finalRotation = glm::slerp(this->rotations[p0Index].rotation, this->rotations[p1Index].rotation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);

	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScale(float animationTime)
{
	if (1 == this->numScales)
	{
		return glm::scale(glm::mat4(1.0f), this->scales[0].scale);
	}

	int p0Index = GetScaleIndex(animationTime);
	int p1Index = p0Index + 1;

	float scaleFactor = GetScaleFactor(this->scales[p0Index].timestamp, this->scales[p1Index].timestamp, animationTime);

	glm::vec3 finalScale = glm::mix(this->scales[p0Index].scale, this->scales[p1Index].scale, scaleFactor);

	return glm::scale(glm::mat4(1.0f), finalScale);
}