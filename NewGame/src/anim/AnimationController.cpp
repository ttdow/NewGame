#include "AnimationController.h"

AnimationController::AnimationController()
{
	this->time = Time::GetInstance();
	this->animations = std::map<std::string, Animation*>();
	this->animators = std::map<std::string, Animator*>();
}

void AnimationController::Update(Shader* shader)
{
	this->currentAnimator->UpdateAnimation(this->time->GetDeltaTime());
	std::vector<glm::mat4> transforms = this->currentAnimator->finalBoneMatrices;
	for (int i = 0; i < transforms.size(); i++)
	{
		shader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
	}
}

void AnimationController::Play(std::string animationName)
{
	this->currentAnimation = this->animations.find(animationName)->second;
	this->currentAnimator = this->animators.find(animationName + "Animator")->second;
}