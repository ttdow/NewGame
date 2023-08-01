#pragma once

class Bone
{

private:

	const static unsigned int MAX_BONES = 4;

public:

	unsigned int boneId[MAX_BONES];
	float weight[MAX_BONES];

};