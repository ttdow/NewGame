#include "RenderingSystem.h"

RenderingSystem::RenderingSystem(Window *window)
{
	// Set OpenGL flags.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Get window data.
	this->window = window;
	this->windowWidth = window->GetWidth();
	this->windowHeight = window->GetHeight();

	// ------------------------ Setup goblin character ---------------------------
	// TODO much of this should be moved to entity/component creation.
	this->goblin = new Entity("goblin");
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 sca = glm::vec3(0.1f, 0.1f, 0.1f);
	this->goblinTransform = new Transform(this->goblin, pos, rot, sca);
	this->goblinTransform->entity = this->goblin;
	this->goblin->components.push_back(this->goblinTransform);

	// Goblin mesh renderer.
	/*
	this->goblinRenderer = new MeshRenderer(this->animShader, this->gobbo);
	this->goblinRenderer->entity = this->goblin;
	this->goblin->components.push_back(this->goblinRenderer);
	*/

	// Perlin Noise Generator.
	/*
	this->noiseMaker = PerlinNoise::GetInstance();
	this->noiseMaker->SetRandomSeed(999);
	*/

	this->dirLight = new DirectionalLight();
	this->lightPos = glm::vec3(0.0f, 20.0f, 10.0f);
	this->dirLight->direction = glm::normalize(glm::vec3(0.0f) - this->lightPos);
	this->dirLight->ambientIntensity = glm::vec3(0.05f, 0.05f, 0.05f);
	this->dirLight->diffuseIntensity = glm::vec3(0.6f, 0.6f, 0.6f);
	this->dirLight->specularIntensity = glm::vec3(0.6f, 0.6f, 0.6f);

	this->pointLight = new PointLight(*this->goblinTransform->position);
	this->pointLight->ambientIntensity = glm::vec3(0.05f, 0.05f, 0.05f);
	this->pointLight->diffuseIntensity = glm::vec3(0.6f, 0.6f, 0.6f);
	this->pointLight->specularIntensity = glm::vec3(0.6f, 0.6f, 0.6f);
	this->pointLight->constant = 0.0f;
	this->pointLight->linear = 0.0f;
	this->pointLight->quadratic = 1.0f;

	// Shaders.
	this->lightShader = new Shader("res/shaders/light.vs", "res/shaders/light.fs");
	this->blinnPhong = new Shader("res/shaders/blinnPhong.vs", "res/shaders/blinnPhong.fs");
	this->simpleDepthShader = new Shader("res/shaders/simpleDepthShader.vs", "res/shaders/simpleDepthShader.fs");
	this->debugDepthShader = new Shader("res/shaders/debug_depth.vs", "res/shaders/debug_depth.fs");
	this->hdrShader = new Shader("res/shaders/hdr.vs", "res/shaders/hdr.fs");
	this->blurShader = new Shader("res/shaders/blur.vs", "res/shaders/blur.fs");
	this->blinnPhong2 = new Shader("res/shaders/blinnPhong.vs", "res/shaders/blinnPhong2.fs");
	this->texturedQuadShader = new Shader("res/shaders/quadTexture.vs", "res/shaders/quadTexture.fs");
	this->viewSpaceShader = new Shader("res/shaders/blinnPhong2.vs", "res/shaders/blinnPhong2.fs");
	this->ssaoMapShader = new Shader("res/shaders/ssao.vs", "res/shaders/ssao.fs");

	/*
	this->ourShader = new Shader("res/shaders/model_loading.vs", "res/shaders/model_loading.fs");
	this->standardShader = new Shader("res/shaders/standard.vs", "res/shaders/standard.fs");
	this->animShader = new Shader("res/shaders/anim.vs", "res/shaders/anim.fs");
	this->blendShader = new Shader("res/shaders/blending.vs", "res/shaders/blending.fs");
	this->volumeLightShader = new Shader("res/shaders/volume_light.vs", "res/shaders/volume_light.fs");
	this->particleShader = new Shader("res/shaders/particle.vs", "res/shaders/particle.fs");
	this->grassShader = new Shader("res/shaders/grass.vs", "res/shaders/grass.fs");
	this->uiShader = new Shader("res/shaders/ui.vs", "res/shaders/ui.fs");
	this->textShader = new Shader("res/shaders/text.vs", "res/shaders/text.fs");
	this->pbrShader = new Shader("res/shaders/pbr.vs", "res/shaders/pbr.fs");
	*/

	// Initialize camera.
	this->camera = new Camera(this->goblinTransform);
	
	// Tracked uniforms.
	this->showDebugShadowMap = false;
	this->showDebugSSAOMap = false;
	this->orthoValues = glm::vec4(-22.5f, 22.5f, -22.5f, 22.5f);
	this->nearPlane = 0.1f;
	this->farPlane = 35.0f;
	this->lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	this->minBias = 0.005f;
	this->shininess = 32.0f;
	this->exposure = 1.0f;
	this->firstGamma = true;
	this->secondGamma = true;
	this->hdr = false;
	this->bloom = false;
	this->bloomBrightness = 1.0f;
	this->debugNormalMap = false;
	this->debugVertexNormals = false;
	this->horizontal = true;
	this->kernelSize = 3;

	this->quadVAO = 0;
	this->sphereVAO = 0;
	
	// Models.
	this->sponza = new ModelClass("res/obj/sponza/sponza.obj");

	/*
	this->treeLevel = new Model("res/obj/tree/tree_level.obj");
	this->torch = new Model("res/obj/torch/torch.obj");
	this->spider = new Model("res/obj/spider.fbx");
	this->tree = new Model("res/obj/old_trees/old_tree.obj");
	
	stbi_set_flip_vertically_on_load(true);
	this->gobbo = new Model("res/obj/gobbo.fbx");
	this->goblinAnimController = new AnimationController();
	this->gobboWalk = new Animation("res/obj/gobbo_walk.fbx", this->gobbo);
	this->gobboWalkAnimator = new Animator(this->gobboWalk);
	this->gobboIdle = new Animation("res/obj/gobbo_idle.fbx", this->gobbo);
	this->gobboIdleAnimator = new Animator(this->gobboIdle);
	this->gobboAttack = new Animation("res/obj/gobbo_attack.fbx", this->gobbo);
	this->gobboAttackAnimator = new Animator(this->gobboAttack);
	this->gobboJump = new Animation("res/obj/gobbo_jump.fbx", this->gobbo);
	this->gobboJumpAnimator = new Animator(this->gobboJump);

	this->spiderAnimController = new AnimationController();
	this->spiderMove = new Animation("res/obj/spider.fbx", this->spider);
	this->spiderMoveAnimator = new Animator(this->spiderMove);

	// Walk animation.
	std::pair<std::string, Animation*> walkAnim = std::pair<std::string, Animation*>();
	walkAnim.first = "walk";
	walkAnim.second = this->gobboWalk;

	this->goblinAnimController->animations.insert(walkAnim);

	std::pair<std::string, Animator*> walkAnimator = std::pair<std::string, Animator*>();
	walkAnimator.first = "walkAnimator";
	walkAnimator.second = this->gobboWalkAnimator;

	this->goblinAnimController->animators.insert(walkAnimator);

	// Idle animation.
	std::pair<std::string, Animation*> idleAnim = std::pair<std::string, Animation*>();
	idleAnim.first = "idle";
	idleAnim.second = this->gobboIdle;

	this->goblinAnimController->animations.insert(idleAnim);

	std::pair<std::string, Animator*> idleAnimator = std::pair<std::string, Animator*>();
	idleAnimator.first = "idleAnimator";
	idleAnimator.second = this->gobboIdleAnimator;

	this->goblinAnimController->animators.insert(idleAnimator);
	this->goblinAnimController->Play("idle");

	// Attack animation.
	std::pair<std::string, Animation*> attackAnim = std::pair<std::string, Animation*>();
	attackAnim.first = "attack";
	attackAnim.second = this->gobboAttack;

	this->goblinAnimController->animations.insert(attackAnim);

	std::pair<std::string, Animator*> attackAnimator = std::pair<std::string, Animator*>();
	attackAnimator.first = "attackAnimator";
	attackAnimator.second = this->gobboAttackAnimator;

	this->goblinAnimController->animators.insert(attackAnimator);

	// Jump animation.
	std::pair<std::string, Animation*> jumpAnim = std::pair<std::string, Animation*>();
	jumpAnim.first = "jump";
	jumpAnim.second = this->gobboJump;

	this->goblinAnimController->animations.insert(jumpAnim);

	std::pair<std::string, Animator*> jumpAnimator = std::pair<std::string, Animator*>();
	jumpAnimator.first = "jumpAnimator";
	jumpAnimator.second = this->gobboJumpAnimator;

	this->goblinAnimController->animators.insert(jumpAnimator);

	// Spider move animation.
	std::pair<std::string, Animation*> spiderAnim = std::pair<std::string, Animation*>();
	spiderAnim.first = "move";
	spiderAnim.second = this->spiderMove;

	this->spiderAnimController->animations.insert(spiderAnim);

	std::pair<std::string, Animator*> spiderAnimator = std::pair<std::string, Animator*>();
	spiderAnimator.first = "moveAnimator";
	spiderAnimator.second = this->spiderMoveAnimator;
	
	this->spiderAnimController->animators.insert(spiderAnimator);
	this->spiderAnimController->Play("move");

	// Particles.
	//this->particleGenerator = new ParticleGenerator(glm::vec3(-39.7893f, 22.0f, 25.2649f), 50, 2.0f);
	*/
	// Floating point framebuffer.
	glGenFramebuffers(1, &this->hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->hdrFBO);

	// Create two output textures.
	glGenTextures(2, this->colorBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, this->colorBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->windowWidth, this->windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// Attach texture to framebuffer.
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->colorBuffer[i], 0);
	}
	
	// Create and attach depth buffer.
	glGenRenderbuffers(1, &this->rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, this->rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->windowWidth, this->windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->rboDepth);

	// Tell OpenGL which color attachments we'll use for rendering.
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// Finally, check if framebuffer is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
	}

	// Ping-pong framebuffers for Gaussian blur.
	glGenFramebuffers(2, this->pingPongFBO);
	glGenTextures(2, this->pingPongColorBuffers);

	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->pingPongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, this->pingPongColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->windowWidth, this->windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->pingPongColorBuffers[i], 0);

		// Check if framebuffers are complete.
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Framebuffer not complete!" << std::endl;
		}
	}

	// Reset to default framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// This isn't called in example.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Other old stuff.
	/*
	// Move goblin entity to initial position.
	this->goblin->GetTransform()->position->z = 150.0f;
	
	// Skybox.
	this->skybox = new Skybox();

	// Trees.
	std::vector<glm::vec3> treePoses;
	treePoses.emplace_back(10.0f, 0.0f, 150.0f);
	treePoses.emplace_back(-20.0f, 1.0f, 134.0f);
	treePoses.emplace_back(55.0f, 1.0f, 176.0f);
	treePoses.emplace_back(200.0f, 1.0f, 50.0f);
	treePoses.emplace_back(43.0f, 1.0f, -150.0f);

	for (unsigned int i = 0; i < treePoses.size(); i++)
	{
		float s = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) + 1.0f);
		float r = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) + 3.14159f * 2.0f);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, treePoses[i]);
		model = glm::rotate(model, r, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(s, s, s));

		this->treeModelMatrices.push_back(model);
	}

	// Grass.
	for (unsigned int i = 0; i < 50000; i++)
	{
		float r0 = (static_cast<float>(rand() / static_cast<float>(RAND_MAX)) - 0.5f) * 500.0f;
		float r1 = (static_cast<float>(rand() / static_cast<float>(RAND_MAX)) - 0.5f) * 500.0f;

		this->vegetation.push_back(glm::vec3(r0, 2.0f, r1));
	}

	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->vegetation.size(), &this->vegetation[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float quadVertices[] =
	{
		// Position				// Texture UVs
		-1.0f,  1.0f,  0.0f,	0.0f,  0.0f,
		-1.0f, -1.0f,  0.0f,	0.0f,  1.0f,
		 1.0f, -1.0f,  0.0f,	1.0f,  1.0f,

		-1.0f,  1.0f,  0.0f,	0.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,	1.0f,  1.0f,
		 1.0f,  1.0f,  0.0f,	1.0f,  0.0f
	};

	glGenVertexArrays(1, &this->blendVAO);
	glGenBuffers(1, &this->blendVBO);
	glBindVertexArray(this->blendVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->blendVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
	glBindVertexArray(0);

	this->grassTex = new TextureClass("res/textures/grass.png", false);
	this->grassTex->SetWrapToClamped();

	this->blendShader->Use();
	this->blendShader->SetInt("texture1", 0);

	this->particleShader->Use();
	this->particleShader->SetInt("texture", 0);
	
	// Collider test.
	this->boxCollider = new BoxCollider();
	glGenVertexArrays(1, &this->boxCollider->vao);
	glGenBuffers(1, &this->boxCollider->vbo);
	glBindVertexArray(this->boxCollider->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->boxCollider->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->boxCollider->vertices), &this->boxCollider->vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	this->boxCollider2 = new BoxCollider();

	this->boxCollider->extents = glm::vec3(0.5f, 0.5f, 0.5f);
	this->boxCollider2->extents = glm::vec3(0.5f, 0.5f, 0.5f);

	// PBR Shader stuff.
	this->pbrShader->Use();
	this->pbrShader->SetInt("albedoMap", 0);
	this->pbrShader->SetInt("normalMap", 1);
	this->pbrShader->SetInt("metallicMap", 2);
	this->pbrShader->SetInt("roughnessMap", 3);
	this->pbrShader->SetInt("aoMap", 4);

	this->rustedMetalMaterial = new Material("res/textures/rusted_metal/");

	this->sphereVAO = 0;

	// Spheres.
	this->indexCount = 0;
	*/

	// Shadow map.
	float planeVertices[] =
	{
		// Position				// Texture UVs
		-1.0f,  1.0f,  0.0f,	1.0f,  1.0f,
		-1.0f, -1.0f,  0.0f,	1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,	0.0f,  0.0f,

		-1.0f,  1.0f,  0.0f,	1.0f,  1.0f,
		 1.0f, -1.0f,  0.0f,	0.0f,  0.0f,
		 1.0f,  1.0f,  0.0f,	0.0f,  1.0f
	};

	glGenVertexArrays(1, &this->planeVAO);
	glGenBuffers(1, &this->planeVBO);
	glBindVertexArray(this->planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	const unsigned int SHADOW_WIDTH = this->windowWidth, SHADOW_HEIGHT = this->windowHeight;
	glGenFramebuffers(1, &this->depthMapFBO);
	glGenTextures(1, &this->depthMap);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER: Framebuffer is not complete!" << std::endl;
	}

	this->debugDepthShader->Use();
	this->debugDepthShader->SetInt("depthMap", 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	CreateGBuffer();

	CreateSSAOData();
}

RenderingSystem::~RenderingSystem()
{
	// Clean up light sources.
	delete(this->dirLight);

	// Clean up shader programs.
	delete(this->ourShader);
	delete(this->standardShader);
	delete(this->animShader);
	delete(this->blendShader);
	delete(this->volumeLightShader);
	delete(this->particleShader);
	delete(this->colliderShader);
	delete(this->grassShader);
	delete(this->uiShader);
	delete(this->textShader);
	delete(this->pbrShader);
	delete(this->simpleDepthShader);
	delete(this->debugDepthShader);
	delete(this->blinnPhong);
	delete(this->lightShader);
	delete(this->hdrShader);
	delete(this->blurShader);
	delete(this->ssaoMapShader);

	// Clean up models.
	delete(this->treeLevel);
	delete(this->torch);
	delete(this->gobbo);
	delete(this->spider);
	delete(this->sponza);

	// Clean up animations.
	delete(this->gobboWalk);
	delete(this->gobboWalkAnimator);
	delete(this->gobboIdle);
	delete(this->gobboIdleAnimator);
	delete(this->gobboJump);
	delete(this->gobboJumpAnimator);
	delete(this->spiderMove);
	delete(this->spiderMoveAnimator);

	// Clean up animation controllers.
	delete(this->goblinAnimController);
	delete(this->spiderAnimController);

	// Clean up entities and components.
	delete(this->goblin);
	delete(this->goblinRenderer);
	delete(this->goblinTransform);

	// Clean up other misc.
	delete(this->camera);
	delete(this->noiseMaker);
	delete(this->particleGenerator);
	delete(this->skybox);

	// Clean up box collider tests.
	delete(this->boxCollider);
	delete(this->boxCollider2);

	// Clean up text rendering test.
	delete(this->uiElement);
}

void RenderingSystem::CreateGBuffer()
{
	// Bind framebuffer.
	glGenFramebuffers(1, &this->gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->gBuffer);

	// Position.
	glGenTextures(1, &this->gPosition);
	glBindTexture(GL_TEXTURE_2D, this->gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->windowWidth, this->windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gPosition, 0);

	// Normal.
	glGenTextures(1, &this->gNormal);
	glBindTexture(GL_TEXTURE_2D, this->gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->windowWidth, this->windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->gNormal, 0);

	// Albedo.
	glGenTextures(1, &this->gAlbedo);
	glBindTexture(GL_TEXTURE_2D, this->gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->windowWidth, this->windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->gAlbedo, 0);

	// Create and attach depth buffer.
	glGenRenderbuffers(1, &this->gDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, this->gDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->windowWidth, this->windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->gDepth);

	// Tell OpenGL about the color attachments.
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Finally, check if framebuffer is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
	}

	// Re-bind default framebuffer and renderbuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingSystem::CreateSSAOData()
{
	// Generate sample kernel.
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	std::default_random_engine generator;
	for (unsigned int i = 0; i < 64; i++)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, 
						 randomFloats(generator) * 2.0 - 1.0, 
						 randomFloats(generator));

		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		
		float scale = float(i) / 64.0f;
		scale = 0.1f + (scale * scale) * (1.0f - 0.1f);
		
		sample *= scale;
		this->ssaoKernel.push_back(sample);
	}

	// Generate noise texture.
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0f - 1.0f,
						randomFloats(generator) * 2.0f - 1.0f,
						0.0f);

		this->ssaoNoise.push_back(noise);
	}

	glGenTextures(1, &this->noiseTexture);
	glBindTexture(GL_TEXTURE_2D, this->noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &this->ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Create SSAO framebuffer.
	glGenFramebuffers(1, &this->ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->ssaoFBO);

	glGenTextures(1, &this->ssaoBuffer);
	glBindTexture(GL_TEXTURE_2D, this->ssaoBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->windowWidth, this->windowHeight, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->ssaoBuffer, 0);
}

void RenderingSystem::RenderQuad()
{
	if (this->quadVAO == 0)
	{
		glGenVertexArrays(1, &this->quadVAO);

		glGenBuffers(1, &this->quadVBO);

		float quadVertices[] =
		{
			// Position				// Texture UVs
			-1.0f,  1.0f,  0.0f,	1.0f,  1.0f,
			-1.0f, -1.0f,  0.0f,	1.0f,  0.0f,
			 1.0f, -1.0f,  0.0f,	0.0f,  0.0f,

			-1.0f,  1.0f,  0.0f,	1.0f,  1.0f,
			 1.0f, -1.0f,  0.0f,	0.0f,  0.0f,
			 1.0f,  1.0f,  0.0f,	0.0f,  1.0f
		};

		glBindVertexArray(this->quadVAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindVertexArray(0);
	}

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void RenderingSystem::RenderSphere()
{
	if (this->sphereVAO == 0)
	{
		glGenVertexArrays(1, &this->sphereVAO);

		glGenBuffers(1, &this->sphereVBO);
		glGenBuffers(1, &this->sphereEBO);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;

		const float PI = 3.14159265359f;

		for (unsigned int x = 0; x <= X_SEGMENTS; x++)
		{
			for (unsigned int y = 0; y <= Y_SEGMENTS; y++)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;

				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; y++)
		{
			if (!oddRow)
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; x++)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; x--)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}

			oddRow = !oddRow;
		}

		this->indexCount = static_cast<unsigned int>(indices.size());

		std::vector<float> data;
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);

			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}

			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
		}

		glBindVertexArray(this->sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->sphereVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphereEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		unsigned int stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	}

	glBindVertexArray(this->sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, this->indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RenderingSystem::CreateShadowMap()
{
	// Configure shadow map FBO.
	glGenFramebuffers(1, &this->shadowMapFBO);
	
	// Create shadow map texture.
	glGenTextures(1, &this->shadowMap);
	glBindTexture(GL_TEXTURE_2D, this->shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, this->windowWidth, this->windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Attach shadow map texture as FBO's depth buffer.
	glBindFramebuffer(GL_FRAMEBUFFER, this->shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER: Framebuffer is not complete!" << std::endl;
	}

	// Re-bind default framebuffer and texture.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 RenderingSystem::GetLightSpaceMatrix()
{
	glm::mat4 lightProjection, lightView, lightSpaceMatrix;

	lightProjection = glm::ortho(this->orthoValues.x, 
								 this->orthoValues.y, 
								 this->orthoValues.z, 
								 this->orthoValues.w, 
								 this->nearPlane, 
								 this->farPlane);

	lightView = glm::lookAt(this->lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;

	return lightSpaceMatrix;
}

void RenderingSystem::ClearCurrentFrame()
{
	glViewport(0, 0, this->windowWidth, this->windowHeight);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderingSystem::UpdateShadowMap()
{
	this->simpleDepthShader->Use();
	this->simpleDepthShader->SetMat4("lightSpaceMatrix", GetLightSpaceMatrix());

	glViewport(0, 0, this->windowWidth, this->windowHeight);
	//glViewport(0, 0, 4096, 4096);
	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);
	this->simpleDepthShader->SetMat4("model", model);

	this->sponza->Draw(*this->simpleDepthShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderingSystem::RenderDebugDepthMap()
{
	// Select shader.
	this->debugDepthShader->Use();

	// Bind depth map texture.
	glActiveTexture(GL_TEXTURE17);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	this->debugDepthShader->SetInt("depthMap", 17);

	// Draw.
	glBindVertexArray(this->planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
	// Bind default texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::RenderToHDRFramebuffer()
{
	// Clear frame.
	glBindFramebuffer(GL_FRAMEBUFFER, this->hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Select shader.
	this->blinnPhong->Use();

	// Bind shadow map texture and set shadow uniforms.
	glActiveTexture(GL_TEXTURE17);
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	this->blinnPhong->SetInt("shadowMap", 17);
	glActiveTexture(GL_TEXTURE0);
	this->blinnPhong->SetMat4("lightSpaceMatrix", GetLightSpaceMatrix());
	this->blinnPhong->SetFloat("minBias", this->minBias);

	// Set scene uniforms.
	this->blinnPhong->SetBool("debugNormalMap", this->debugNormalMap);
	this->blinnPhong->SetBool("debugVertexNormals", this->debugVertexNormals);
	this->blinnPhong->SetFloat("bloomBrightness", this->bloomBrightness);
	this->blinnPhong->SetBool("firstGamma", this->firstGamma);
	this->blinnPhong->SetCamera(this->camera, this->windowWidth, this->windowHeight);
	this->dirLight->direction = glm::normalize(glm::vec3(0.0f) - this->lightPos);
	this->blinnPhong->SetVec3("dirLight.direction", this->dirLight->direction);
	this->blinnPhong->SetVec3("dirLight.ambient", this->dirLight->ambientIntensity);
	this->blinnPhong->SetVec3("dirLight.diffuse", this->dirLight->diffuseIntensity);
	this->blinnPhong->SetVec3("dirLight.specular", this->dirLight->specularIntensity);
	this->blinnPhong->SetVec3("pointLight.position", this->pointLight->position);
	this->blinnPhong->SetVec3("pointLight.ambient", this->pointLight->ambientIntensity);
	this->blinnPhong->SetVec3("pointLight.diffuse", this->pointLight->diffuseIntensity);
	this->blinnPhong->SetVec3("pointLight.specular", this->pointLight->specularIntensity);
	this->blinnPhong->SetFloat("pointLight.constant", this->pointLight->constant);
	this->blinnPhong->SetFloat("pointLight.linear", this->pointLight->linear);
	this->blinnPhong->SetFloat("pointLight.quadratic", this->pointLight->quadratic);
	this->blinnPhong->SetFloat("shininess", this->shininess);
	this->blinnPhong->SetMat4("model", glm::mat4(1.0f));
	this->blinnPhong->SetInt("kernelSize", this->kernelSize);

	// Draw model.
	this->sponza->Draw(*this->blinnPhong);

	// Set sphere uniforms for ball player model.
	this->lightShader->Use();
	this->lightShader->SetCamera(this->camera, this->windowWidth, this->windowHeight);
	this->lightShader->SetMat4("model", this->goblin->GetTransform()->modelMatrix);
	this->lightShader->SetVec3("color", glm::vec3(0.0f, 0.0f, 100.0f));

	// Draw sphere.
	RenderSphere();

	// Draw directional light source (i.e. sun).
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->lightPos);
	this->lightShader->SetMat4("model", model);
	this->lightShader->SetVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));

	// Draw sphere.
	RenderSphere();

	// Bind default framebuffer and texture.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::RenderToBloomFramebuffer()
{
	this->horizontal = true;
	bool firstIteration = true;
	unsigned amount = 10;

	// Select shader.
	this->blurShader->Use();

	// Load bright-object texture.
	this->blurShader->SetInt("image", 0);
	glActiveTexture(GL_TEXTURE0);

	// Blur bright objects using Gaussian blur to create Bloom effect.
	for (unsigned int i = 0; i < amount; i++)
	{
		// Bind current ping-pong framebuffer.
		glBindFramebuffer(GL_FRAMEBUFFER, this->pingPongFBO[this->horizontal]);
		this->blurShader->SetBool("horizontal", this->horizontal);

		// Bind the texture from the previous ping-pong render.
		glBindTexture(GL_TEXTURE_2D, firstIteration ? this->colorBuffer[1] : this->pingPongColorBuffers[!this->horizontal]);

		// Render the textured quad.
		RenderQuad();

		// Ping-pong.
		this->horizontal = !this->horizontal;

		// Toggle from color buffer to pinp-pong color buffers after first.
		if (firstIteration)
		{
			firstIteration = false;
		}
	}

	// Unbind framebuffer and texture.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::RenderToScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Select shader.
	this->hdrShader->Use();

	// Load texture from HDR framebuffer.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->colorBuffer[0]); // Regular scene.
	this->hdrShader->SetInt("hdrBuffer", 0);

	// Load texture from Bloom framebuffer.
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->pingPongColorBuffers[!this->horizontal]); // Blurred bright colors.
	this->hdrShader->SetInt("bloomBlur", 1);

	// Set uniforms.
	this->hdrShader->SetBool("bloom", this->bloom);
	this->hdrShader->SetBool("hdr", this->hdr);
	this->hdrShader->SetFloat("exposure", this->exposure);
	this->hdrShader->SetBool("secondGamma", this->secondGamma);

	// Draw to full-screen quad.
	RenderQuad();

	// Unbind textures.
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::RenderToQuad()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Select shader.
	this->texturedQuadShader->Use();

	// Load textures from G-Buffer.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->gPosition);
	this->texturedQuadShader->SetInt("gPosition", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->gNormal);
	this->texturedQuadShader->SetInt("gNormal", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->gAlbedo);
	this->texturedQuadShader->SetInt("gAlbedo", 2);

	// Camera uniforms.
	this->texturedQuadShader->SetCamera(this->camera, this->windowWidth, this->windowHeight);

	// Light uniforms.
	for (unsigned int i = 0; i < 12; i++)
	{
		std::string name = "lightPos[" + std::to_string(i) + "]";
		if (i < 6)
		{
			this->texturedQuadShader->SetVec3(name.c_str(), glm::vec3(-9.8f + (i * 4.05f), 4.2f, 4.75f));
		}
		else
		{
			this->texturedQuadShader->SetVec3(name.c_str(), glm::vec3(-9.8f + ((i - 6) * 4.05f), 4.2f, -4.79f));
		}
	}

	// Draw to full-screen quad.
	RenderQuad();

	// Unbind texture.
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::RenderToGBuffer(Shader* shader)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Select shader.
	shader->Use();

	// Set scene uniforms.
	shader->SetCamera(this->camera, this->windowWidth, this->windowHeight);
	shader->SetMat4("lightSpaceMatrix", GetLightSpaceMatrix());
	shader->SetMat4("model", glm::mat4(1.0f));

	// Draw model.
	this->sponza->Draw(*shader);

	// Set sphere uniforms for ball player model.
	this->lightShader->Use();
	this->lightShader->SetCamera(this->camera, this->windowWidth, this->windowHeight);
	this->lightShader->SetMat4("model", this->goblin->GetTransform()->modelMatrix);
	this->lightShader->SetVec3("color", glm::vec3(0.0f, 0.0f, 100.0f));

	// Draw sphere.
	RenderSphere();

	// Draw directional light source (i.e. sun).
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->lightPos);
	this->lightShader->SetMat4("model", model);
	this->lightShader->SetVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));

	// Draw sphere.
	RenderSphere();

	// Bind default framebuffer and texture.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::RenderDebugSSAOMap()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Select shader.
	this->ssaoMapShader->Use();

	// Load textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->gPosition);
	this->ssaoMapShader->SetInt("gPosition", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->gNormal);
	this->ssaoMapShader->SetInt("gNormal", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->noiseTexture);
	this->ssaoMapShader->SetInt("texNoise", 2);

	// Camera uniforms.
	this->ssaoMapShader->SetCamera(this->camera, this->windowWidth, this->windowHeight);

	// Window uniforms.
	this->ssaoMapShader->SetFloat("windowWidth", static_cast<float>(this->windowWidth));
	this->ssaoMapShader->SetFloat("windowHeight", static_cast<float>(this->windowHeight));

	// Kernel samples.
	for (unsigned int i = 0; i < this->ssaoKernel.size(); i++)
	{
		std::string name = "samples[" + std::to_string(i) + "]";
		this->ssaoMapShader->SetVec3(name, this->ssaoKernel[i]);
	}

	// Draw to full-screen quad.
	RenderQuad();

	// Unbind texture.
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::Update()
{
	ClearCurrentFrame();

	// 1. Render shadow map to texture.
	UpdateShadowMap();

	ClearCurrentFrame();

	// 1.a. Render shadow map texture on full-screen quad.
	if (this->showDebugShadowMap)
	{
		RenderDebugDepthMap();
		return;
	}

	// Update camera.
	this->camera->Update();

	// Use deferred rendering or forward rendering.
	if (this->useGBuffer)
	{
		if (this->showDebugSSAOMap)
		{
			RenderToGBuffer(this->viewSpaceShader);

			RenderDebugSSAOMap();
			return;
		}
		// 2. Render scene to G-Buffer.
		RenderToGBuffer(this->blinnPhong2);

		// 3. Render texure to full-screen quad.
		RenderToQuad();
	}
	else
	{
		// 2.a. Render scene to HDR framebuffer.
		RenderToHDRFramebuffer();

		// 2.b. Render scene to Bloom framebuffer.
		RenderToBloomFramebuffer();

		// 3. Combine both framebuffers and render to full-screen quad.
		RenderToScreen();
	}

	// Old stuff.
	/*
	// Draw goblin.
	this->goblin->GetRenderer()->AnimUpdate(projection,
										    view,
											camera->position,
											lightDir,
											lightAmbient,
											lightDiffuse,
											lightSpecular,
											this->goblinAnimController,
											pointLightPos,
											pointLightAmbient,
											pointLightDiffuse,
											pointLightSpecular,
											pointLightIntensity,
											lightSpaceMatrix);

	// Draw spider.
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 18.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	this->animShader->Use();
	this->animShader->SetMat4("model", model);
	this->animShader->SetMat4("view", view);
	this->animShader->SetMat4("projection", projection);
	this->animShader->SetVec3("viewPos", camera->position);
	this->animShader->SetFloat("material.shininess", 32.0);
	this->animShader->SetInt("material.diffuse", 0);
	this->animShader->SetInt("material.specular", 0);
	this->animShader->SetVec3("directionalLight.direction", -lightDir);
	this->animShader->SetVec3("directionalLight.ambient", lightAmbient);
	this->animShader->SetVec3("directionalLight.diffuse", lightDiffuse);
	this->animShader->SetVec3("directionalLight.specular", lightSpecular);
	this->animShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	this->animShader->SetVec3("pointLight.position", pointLightPos + glm::vec3(0.0f, 5.0f, 0.0f));
	this->animShader->SetVec3("pointLight.ambient", pointLightAmbient);
	this->animShader->SetVec3("pointLight.diffuse", pointLightDiffuse);
	this->animShader->SetVec3("pointLight.specular", pointLightSpecular);
	this->animShader->SetFloat("pointLight.constant", 1.0f);
	this->animShader->SetFloat("pointLight.linear", 0.1f);
	this->animShader->SetFloat("pointLight.quadratic", 0.001f);
	this->animShader->SetFloat("pointLight.intensity", pointLightIntensity);

	this->spiderAnimController->Update(this->animShader);

	this->spider->Draw(*this->animShader);

	// Draw the tree level.
	model = glm::mat4(1.0f);
	this->standardShader->Use();
	this->standardShader->SetMat4("model", model);
	this->standardShader->SetMat4("view", view);
	this->standardShader->SetMat4("projection", projection);
	this->standardShader->SetVec3("viewPos", camera->position);
	this->standardShader->SetFloat("material.shininess", 32.0);
	this->standardShader->SetInt("material.diffuse", 0);
	this->standardShader->SetInt("material.specular", 0);
	this->standardShader->SetVec3("directionalLight.direction", lightDir);
	this->standardShader->SetVec3("directionalLight.ambient", lightAmbient);
	this->standardShader->SetVec3("directionalLight.diffuse", lightDiffuse);
	this->standardShader->SetVec3("directionalLight.specular", lightSpecular);
	this->standardShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	this->standardShader->SetVec3("pointLight.position", pointLightPos + glm::vec3(0.0f, 5.0f, 0.0f));
	this->standardShader->SetVec3("pointLight.ambient", pointLightAmbient);
	this->standardShader->SetVec3("pointLight.diffuse", pointLightDiffuse);
	this->standardShader->SetVec3("pointLight.specular", pointLightSpecular);
	this->standardShader->SetFloat("pointLight.constant", 1.0f);
	this->standardShader->SetFloat("pointLight.linear", 0.1f);
	this->standardShader->SetFloat("pointLight.quadratic", 0.001f);
	this->standardShader->SetFloat("pointLight.intensity", pointLightIntensity);

	this->treeLevel->Draw(*this->standardShader);

	// Draw small trees.
	for (unsigned int i = 0; i < this->treeModelMatrices.size(); i++)
	{
		this->standardShader->SetMat4("model", this->treeModelMatrices[i]);
		this->tree->Draw(*this->standardShader);
	}

	// TODO Volumetric lighting test.
	
	model = glm::mat4(1.0f);
	this->volumeLightShader->Use();
	this->volumeLightShader->SetMat4("model", model);
	this->volumeLightShader->SetMat4("view", view);
	this->volumeLightShader->SetMat4("projection", projection);
	this->volumeLightShader->SetVec3("viewPos", camera->position);
	this->volumeLightShader->SetFloat("material.shininess", 32.0);
	this->volumeLightShader->SetInt("material.diffuse", 0);
	this->volumeLightShader->SetInt("material.specular", 0);
	this->volumeLightShader->SetVec3("light.position", lightPos);
	this->volumeLightShader->SetVec3("light.ambient", lightAmbient);
	this->volumeLightShader->SetVec3("light.diffuse", lightDiffuse);
	this->volumeLightShader->SetVec3("light.specular", lightSpecular);

	this->treeLevel->Draw(*this->volumeLightShader);
	
	// Grass.
	this->grassShader->Use();
	this->grassShader->SetMat4("projection", projection);
	this->grassShader->SetMat4("view", view);
	this->grassShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	this->standardShader->SetVec3("lightDir", lightDir);
	glBindVertexArray(this->blendVAO);
	glBindTexture(GL_TEXTURE_2D, this->grassTex->GetID());
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->vegetation.size());
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	// Flames.
	this->particleGenerator->Update();
	this->particleShader->Use();
	this->particleShader->SetMat4("projection", projection);
	this->particleShader->SetMat4("view", view);
	
	glBindVertexArray(this->blendVAO);
	//glBindTexture(GL_TEXTURE_2D, this->particleTexture);

	for (unsigned int i = 0; i < this->particleGenerator->particles.size(); i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, this->particleGenerator->particles[i].position);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));
		this->particleShader->SetMat4("model", model);
		this->particleShader->SetVec4("color", this->particleGenerator->particles[i].color);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);

	// UI elements.

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.75f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.25f, 1.0f, 0.5f));
	glm::vec4 color = glm::vec4(0.125f, 0.125f, 0.125f, 1.0f);
	this->uiShader->Use();
	this->uiShader->SetMat4("model", model);
	this->uiShader->SetVec4("color", color);
	glBindVertexArray(this->blendVAO);
	glBindTexture(GL_TEXTURE_2D, this->particleTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	// Text.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(this->windowWidth), 0.0f, static_cast<float>(this->windowHeight));
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	this->textShader->Use();
	this->textShader->SetMat4("projection", orthoProjection);
	this->textShader->SetMat4("model", model);
	this->uiElement->RenderText(this->textShader, "X", 100.0f, 100.0f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	
	// Torches.
	model = glm::translate(glm::mat4(1.0f), pointLightPos + glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	this->standardShader->Use();
	this->standardShader->SetMat4("model", model);
	this->standardShader->SetMat4("view", view);
	this->standardShader->SetMat4("projection", projection);
	this->standardShader->SetVec3("viewPos", camera->position);
	this->standardShader->SetFloat("material.shininess", 32.0);
	this->standardShader->SetInt("material.diffuse", 0);
	this->standardShader->SetInt("material.specular", 0);
	this->standardShader->SetVec3("directionalLight.direction", lightDir);
	this->standardShader->SetVec3("directionalLight.ambient", lightAmbient);
	this->standardShader->SetVec3("directionalLight.diffuse", lightDiffuse);
	this->standardShader->SetVec3("directionalLight.specular", lightSpecular);
	this->standardShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	this->standardShader->SetVec3("pointLight.position", pointLightPos);
	this->standardShader->SetVec3("pointLight.ambient", pointLightAmbient);
	this->standardShader->SetVec3("pointLight.diffuse", pointLightDiffuse);
	this->standardShader->SetVec3("pointLight.specular", pointLightSpecular);
	this->standardShader->SetFloat("pointLight.constant", 1.0f);
	this->standardShader->SetFloat("pointLight.linear", 0.045f);
	this->standardShader->SetFloat("pointLight.quadratic", 0.0075f);
	this->standardShader->SetFloat("pointLight.intensity", pointLightIntensity);

	this->torch->Draw(*this->standardShader);
	
	// Collider 1 visualization.
	this->boxCollider->center = glm::vec3(this->goblinTransform->modelMatrix[3][0],
										  this->goblinTransform->modelMatrix[3][1] + 5.0f,
										  this->goblinTransform->modelMatrix[3][2] + 2.0f);

	model = glm::mat4(1.0f);
	model = glm::translate(model, this->boxCollider->center);

	this->colliderShader->Use();
	this->colliderShader->SetMat4("model", model);
	this->colliderShader->SetMat4("view", view);
	this->colliderShader->SetMat4("projection", projection);
	this->colliderShader->SetVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
	glBindVertexArray(this->boxCollider->vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// Collider 2 visualization.
	this->boxCollider2->center = glm::vec3(5.0f, 5.0f, 2.0f);

	model = glm::mat4(1.0f);
	model = glm::translate(model, this->boxCollider2->center);
	this->colliderShader->Use();
	this->colliderShader->SetMat4("model", model);
	this->colliderShader->SetMat4("view", view);
	this->colliderShader->SetMat4("projection", projection);
	this->colliderShader->SetVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
	glBindVertexArray(this->boxCollider->vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// Test for box collider collisions.
	this->boxCollider->CheckCollision(this->boxCollider2);

	// PBR material shader test.
	glm::vec3 lightPositions[] =
	{
		glm::vec3(-10.0f, 10.0f, 160.0f),
		glm::vec3(10.0f, 10.0f, 160.0f),
		glm::vec3(-10.0f, -10.0f, 160.0f),
		glm::vec3(10.0f, -10.0f, 160.0f)
	};

	glm::vec3 lightColors[] =
	{
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

	int nrRows = 7;
	int nrCols = 7;
	float spacing = 2.5f;

	this->pbrShader->Use();
	this->pbrShader->SetCamera(this->camera, windowWidth, windowHeight);
	this->rustedMetalMaterial->BindMaterial();
	
	for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++)
	{
		this->pbrShader->SetVec3("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
		this->pbrShader->SetVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
	}

	// Render rows * columns number of spheres with varying metallic/roughness values scaled by rows and columns respectively.
	model = glm::mat4(1.0f);
	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3((col - (nrCols / 2)) * spacing, (row - (nrRows / 2)) * spacing + 10.0f, 150.0f));

			this->pbrShader->SetMat4("model", model);
			this->pbrShader->SetMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));

			RenderSphere();
		}
	}

	// Draw skybox last.
	this->skybox->Draw(view, projection);
	*/
}

Camera* RenderingSystem::GetCamera()
{
	return this->camera;
}

bool RenderingSystem::ResizeFramebuffer(int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;

	glViewport(0, 0, width, height);

	return true;
}

void RenderingSystem::ChangeAnimation(std::string animationName)
{
	this->goblinAnimController->Play(animationName);
}