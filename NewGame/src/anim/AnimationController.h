#pragma once

#include "../render/Animation.h"
#include "../render/Animator.h"
#include "../util/Time.h"
#include "../render/Shader.h"

#include <map>
#include <string>

class AnimationController
{
public:

	std::map<std::string, Animation*> animations;
	std::map<std::string, Animator*> animators;

	Animator* currentAnimator;

	AnimationController();
	void Update(Shader* shader);
	void Play(std::string animationName);

private:

	Time* time;
	Animation* currentAnimation;
};