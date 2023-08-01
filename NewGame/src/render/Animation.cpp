# include "Animation.h"

Animation::Animation(const std::string& animationPath, Model* model)
{
	this->deltaTime = 0.0f;
	this->currentFrame = 0;

	this->rootTransformation = glm::mat4(1.0f);
	this->childTransformation = glm::mat4(1.0f);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);

	aiAnimation* animation = scene->mAnimations[0];
	//std::cout << "mNumChannels = " << animation->mNumChannels << std::endl;
	unsigned int counter = 0;
	
	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		//std::cout << animation->mChannels[i]->mNodeName.data << std::endl;
		//std::cout << "RotationKeys: " << animation->mChannels[i]->mNumRotationKeys << std::endl;

		for (unsigned int j = 0; j < animation->mChannels[i]->mNumRotationKeys; j++)
		{
			// TODO Print rotation keys (quats) for debugging.
			/*
			std::cout << j << ": (";
			std::cout << animation->mChannels[i]->mRotationKeys[j].mValue.x << ", ";
			std::cout << animation->mChannels[i]->mRotationKeys[j].mValue.y << ", ";
			std::cout << animation->mChannels[i]->mRotationKeys[j].mValue.z << ", ";
			std::cout << animation->mChannels[i]->mRotationKeys[j].mValue.w << ")" << std::endl;
			*/

			if (counter < 4)
			{
				counter++;
				continue;
			}

			glm::quat quat;
			quat.x = animation->mChannels[i]->mRotationKeys[j].mValue.x;
			quat.y = animation->mChannels[i]->mRotationKeys[j].mValue.y;
			quat.z = animation->mChannels[i]->mRotationKeys[j].mValue.z;
			quat.w = animation->mChannels[i]->mRotationKeys[j].mValue.w;

			//this->rotations
			glm::mat4 rotate = glm::toMat4(quat);

			this->rotations.push_back(rotate);
		}
	}

	// TODO Print rotation matrices for debugging.
	/*
	std::cout << "rotations.size() = " << this->rotations.size() << std::endl;
	for (unsigned int i = 0; i < this->rotations.size(); i++)
	{
		std::cout << i << std::endl;
		glm::mat4 rotate = this->rotations[i];

		for (unsigned int j = 0; j < 4; j++)
		{
			std::cout << "   | ";

			for (unsigned int k = 0; k < 4; k++)
			{
				std::cout << rotate[k][j] << " ";
			}

			std::cout << " |" << std::endl;
		}
	}
	*/

	double duration = animation->mDuration;
	//std::cout << "duration = " << duration << std::endl;
	double ticksPerSecond = animation->mTicksPerSecond;
	//std::cout << "ticksPerSecond = " << ticksPerSecond << std::endl;
	aiNode* rootNode = scene->mRootNode;
	std::string name = rootNode->mName.data;
	//std::cout << name << std::endl;

	for (unsigned int i = 0; i < rootNode->mNumChildren; i++)
	{
		std::string childName = rootNode->mChildren[i]->mName.data;
		//std::cout << childName << std::endl;
		//std::cout << "NumChildren: " << rootNode->mChildren[i]->mNumChildren << std::endl;

		for (unsigned int j = 0; j < rootNode->mChildren[i]->mNumChildren; j++)
		{
			//std::cout << rootNode->mChildren[i]->mChildren[j]->mName.data << std::endl;
			this->rootTransformation = ConvertMatrixToGLMFormat(rootNode->mChildren[i]->mChildren[j]->mTransformation);
			//std::cout << rootNode->mChildren[i]->mChildren[j]->mNumChildren << std::endl;
			//std::cout << rootNode->mChildren[i]->mChildren[j]->mChildren[0]->mName.data << std::endl;
			this->childTransformation = ConvertMatrixToGLMFormat(rootNode->mChildren[i]->mChildren[j]->mChildren[0]->mTransformation);
		}
	}

	// TODO Display initial transformation matrices for debugging.
	/*
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			std::cout << this->rootTransformation[i][j] << ", ";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			std::cout << this->childTransformation[i][j] << ", ";
		}

		std::cout << std::endl;
	}
	*/
}

unsigned int Animation::GetAnimationFrame()
{
	Time* time = Time::GetInstance();

	this->deltaTime += time->GetDeltaTime();

	float tickTime = 1.0f / 24.0f; // 0.0416667 s

	if (this->deltaTime >= tickTime)
	{
		this->deltaTime = 0.0f;
		this->currentFrame++;
		if (this->currentFrame >= this->rotations.size())
		{
			this->currentFrame = 0;
		}
	}

	return this->currentFrame;
}

glm::mat4 Animation::ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
{
	glm::mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}