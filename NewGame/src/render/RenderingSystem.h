#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "../util/Window.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "CubeVertices.h"
#include "Animation.h"
#include "Animator.h"
#include "../anim/AnimationController.h"

#include "../ecs/Entity.h"
#include "../ecs/Transform.h"
#include "MeshRenderer.h"

class RenderingSystem
{
private:

	Window* window;
	Camera* camera;

	Shader* ourShader;
	Shader* standardShader;
	Shader* animShader;
	Shader* skyboxShader;
	Shader* blendShader;

	Model* ourModel;
	Model* modelTwo;
	Model* gobbo;

	AnimationController* goblinAnimController;
	Animation* gobboWalk;
	Animator* gobboWalkAnimator;
	Animation* gobboIdle;
	Animator* gobboIdleAnimator;
	Animation* gobboAttack;
	Animator* gobboAttackAnimator;

	unsigned int textureId;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int blendVAO, blendVBO;
	unsigned int grassTexture;

	int windowWidth;
	int windowHeight;

	std::vector<glm::vec3> vegetation;

public:
	
	Model* peachCastle;

	MeshRenderer* testRenderer;
	Entity* backpack;
	Transform* testTransform;

	Entity* goblin;
	MeshRenderer* goblinRenderer;
	Transform* goblinTransform;

	RenderingSystem(Window* window);
	~RenderingSystem();
	
	void Update();
	Camera* GetCamera();
	bool ResizeFramebuffer(int width, int height);
	void ChangeAnimation(std::string animationName);
};