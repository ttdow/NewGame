#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "stb_image.h"
#include "../util/Window.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "CubeVertices.h"
#include "Animation.h"
#include "Animator.h"
#include "../anim/AnimationController.h"
#include "../util/PerlinNoise.h"
#include "../particle/ParticleGenerator.h"
#include "../phys/BoxCollider.h"

#include "../ecs/Entity.h"
#include "../ecs/Transform.h"
#include "MeshRenderer.h"

class RenderingSystem
{
private:

	Window* window;
	Camera* camera;

	BoxCollider* boxCollider;
	BoxCollider* boxCollider2;

	Shader* ourShader;
	Shader* standardShader;
	Shader* animShader;
	Shader* skyboxShader;
	Shader* blendShader;
	Shader* volumeLightShader;
	Shader* particleShader;
	Shader* colliderShader;

	ParticleGenerator* particleGenerator;

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
	unsigned int particleTexture;

	int windowWidth;
	int windowHeight;

	std::vector<glm::vec3> vegetation;

	PerlinNoise* noiseMaker;

public:
	
	Model* peachCastle;
	Model* treeLevel;
	Model* torch;

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