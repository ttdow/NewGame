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
#include "Material.h"
#include "Skybox.h"
#include "ModelClass.h"
#include "TextureManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"

#include "../ecs/Entity.h"
#include "../ecs/Transform.h"
#include "MeshRenderer.h"

class RenderingSystem
{
private:

	Window* window;
	Camera* camera;

	Skybox* skybox;

	BoxCollider* boxCollider;
	BoxCollider* boxCollider2;

	Shader* ourShader;
	Shader* standardShader;
	Shader* animShader;
	Shader* blendShader;
	Shader* volumeLightShader;
	Shader* particleShader;
	Shader* colliderShader;
	Shader* grassShader;
	Shader* uiShader;
	Shader* textShader;
	Shader* pbrShader;
	Shader* simpleDepthShader;
	Shader* debugDepthShader;
	Shader* blinnPhong;
	Shader* lightShader;
	Shader* hdrShader;
	Shader* blurShader;
	Shader* blinnPhong2;
	Shader* texturedQuadShader;
	Shader* ssaoMapShader;
	Shader* viewSpaceShader;

	UIElement* uiElement;

	TextureClass* grassTex;
	Material* rustedMetalMaterial;

	ParticleGenerator* particleGenerator;

	Model* gobbo;
	Model* spider;
	Model* torch;
	Model* tree;
	
	ModelClass* sponza;
	ModelClass* lantern;

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

	unsigned int blendVAO, blendVBO;
	unsigned int sphereVAO, sphereVBO, sphereEBO;
	unsigned int quadVAO, quadVBO;
	unsigned int indexCount;

	unsigned int depthMapFBO;
	unsigned int depthMap;
	unsigned int planeVAO, planeVBO;
	unsigned int shadowMapFBO;
	unsigned int shadowMap;

	unsigned int hdrFBO;
	unsigned int colorBuffer[2];
	unsigned int rboDepth;
	unsigned int pingPongFBO[2];
	unsigned int pingPongColorBuffers[2];
	
	unsigned int gBuffer;
	unsigned int gPosition;
	unsigned int gNormal;
	unsigned int gAlbedo;
	unsigned int gDepth;

	unsigned int noiseTexture;
	unsigned int ssaoFBO;
	unsigned int ssaoBuffer;
	unsigned int ssaoBlurFBO;

	bool horizontal;

	int windowWidth;
	int windowHeight;

	std::vector<glm::vec3> vegetation;
	std::vector<glm::mat4> treeModelMatrices;
	std::vector<glm::vec3> ssaoKernel;
	std::vector<glm::vec3> ssaoNoise;

	PerlinNoise* noiseMaker;

	void CreateGBuffer();
	void CreateSSAOData();
	void RenderQuad();
	void RenderSphere();
	void CreateShadowMap();
	glm::mat4 GetLightSpaceMatrix();
	void ClearCurrentFrame();
	void UpdateShadowMap();
	void RenderDebugDepthMap();
	void RenderToHDRFramebuffer();
	void RenderToBloomFramebuffer();
	void RenderToScreen();
	void RenderToGBuffer(Shader* shader);
	void RenderDebugSSAOMap();
	void RenderToQuad();

public:
	
	bool showDebugShadowMap;
	glm::vec3 lightPos;
	glm::vec4 orthoValues;
	float nearPlane;
	float farPlane;
	glm::vec3 lightColor;
	float minBias;
	bool firstGamma;
	bool secondGamma;
	bool hdr;
	float shininess;
	float exposure;
	bool bloom;
	float bloomBrightness;
	bool debugNormalMap;
	bool debugVertexNormals;
	bool useGBuffer;
	int kernelSize;
	bool showDebugSSAOMap;

	DirectionalLight* dirLight;
	PointLight* pointLight;

	Model* treeLevel;

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