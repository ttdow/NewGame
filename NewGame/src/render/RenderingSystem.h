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
#include "../ui/UIElement.h"
#include "Texture.h"

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
	Shader* grassShader;
	Shader* uiShader;
	Shader* textShader;
	Shader* pbrShader;

	UIElement* uiElement;

	TextureClass* grassTex;
	TextureClass* albedo;
	TextureClass* normal;
	TextureClass* metallic;
	TextureClass* roughness;
	TextureClass* ao;

	ParticleGenerator* particleGenerator;

	Model* gobbo;
	Model* spider;

	AnimationController* goblinAnimController;
	Animation* gobboWalk;
	Animator* gobboWalkAnimator;
	Animation* gobboIdle;
	Animator* gobboIdleAnimator;
	Animation* gobboAttack;
	Animator* gobboAttackAnimator;
	Animation* gobboJump;
	Animator* gobboJumpAnimator;

	AnimationController* spiderAnimController;
	Animation* spiderMove;
	Animator* spiderMoveAnimator;

	unsigned int textureId;
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int blendVAO, blendVBO;
	unsigned int grassTexture;
	unsigned int particleTexture;
	unsigned int sphereVAO, sphereVBO, sphereEBO;
	unsigned int indexCount;

	int windowWidth;
	int windowHeight;

	std::vector<glm::vec3> vegetation;

	PerlinNoise* noiseMaker;

	void RenderSphere();

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