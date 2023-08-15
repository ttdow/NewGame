#include "RenderingSystem.h"

RenderingSystem::RenderingSystem(Window *window)
{
	glEnable(GL_DEPTH_TEST);

	this->noiseMaker = PerlinNoise::GetInstance();
	this->noiseMaker->SetRandomSeed(999);

	this->window = window;
	this->windowWidth = window->GetWidth();
	this->windowHeight = window->GetHeight();

	// Shaders.
	this->ourShader = new Shader("res/shaders/model_loading.vs", "res/shaders/model_loading.fs");
	this->standardShader = new Shader("res/shaders/standard.vs", "res/shaders/standard.fs");
	this->animShader = new Shader("res/shaders/anim.vs", "res/shaders/anim.fs");
	this->skyboxShader = new Shader("res/shaders/skybox.vs", "res/shaders/skybox.fs");
	this->blendShader = new Shader("res/shaders/blending.vs", "res/shaders/blending.fs");
	this->volumeLightShader = new Shader("res/shaders/volume_light.vs", "res/shaders/volume_light.fs");
	this->particleShader = new Shader("res/shaders/particle.vs", "res/shaders/particle.fs");
	this->colliderShader = new Shader("res/shaders/collider.vs", "res/shaders/collider.fs");
	this->grassShader = new Shader("res/shaders/grass.vs", "res/shaders/grass.fs");
	this->uiShader = new Shader("res/shaders/ui.vs", "res/shaders/ui.fs");
	this->textShader = new Shader("res/shaders/text.vs", "res/shaders/text.fs");
	this->pbrShader = new Shader("res/shaders/pbr.vs", "res/shaders/pbr.fs");

	// UI Elements.
	this->uiElement = new UIElement();

	// Particles.
	this->particleGenerator = new ParticleGenerator(glm::vec3(-45.0f, 24.0f, -89.0f), 50, 2.0f);
	
	// Models.
	this->treeLevel = new Model("res/obj/tree/tree_level.obj");
	this->torch = new Model("res/obj/torch/torch.obj");
	this->spider = new Model("res/obj/spider.fbx");

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

	// Setup goblin character.
	// TODO much of this should be moved to entity/component creation.
	this->goblin = new Entity("goblin");
	this->goblin->components.reserve(2);

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 sca = glm::vec3(0.1f, 0.1f, 0.1f);
	this->goblinTransform = new Transform(this->goblin, pos, rot, sca);
	this->goblinTransform->entity = this->goblin;
	this->goblin->components.push_back(this->goblinTransform);

	this->goblinRenderer = new MeshRenderer(this->animShader, this->gobbo);
	this->goblinRenderer->entity = this->goblin;
	this->goblin->components.push_back(this->goblinRenderer);

	// Initialize camera.
	this->camera = new Camera(this->goblinTransform);

	// Move gobline entity to initial position.
	this->goblin->GetTransform()->position->z = 150.0f;

	// Skybox.
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &this->skyboxVAO);
	glGenBuffers(1, &this->skyboxVBO);
	glBindVertexArray(this->skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	this->skyboxShader->Use();
	this->skyboxShader->SetInt("skybox", 0);

	std::vector<std::string> cubeMapTextures;
	cubeMapTextures.push_back("res/textures/skybox/right.jpg");
	cubeMapTextures.push_back("res/textures/skybox/left.jpg");
	cubeMapTextures.push_back("res/textures/skybox/top.jpg");
	cubeMapTextures.push_back("res/textures/skybox/bottom.jpg");
	cubeMapTextures.push_back("res/textures/skybox/front.jpg");
	cubeMapTextures.push_back("res/textures/skybox/back.jpg");

	glGenTextures(1, &this->textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width, height, channels;
	unsigned char* data;
	for (unsigned int i = 0; i < cubeMapTextures.size(); i++)
	{
		stbi_set_flip_vertically_on_load(false);
		data = stbi_load(cubeMapTextures[i].c_str(), &width, &height, &channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << cubeMapTextures[i] << std::endl;
		}

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
}

RenderingSystem::~RenderingSystem()
{
	delete(this->ourShader);
	delete(this->standardShader);
	delete(this->animShader);
	delete(this->skyboxShader);
	delete(this->blendShader);
	delete(this->volumeLightShader);
	delete(this->particleShader);
	delete(this->colliderShader);
	delete(this->grassShader);
	delete(this->uiShader);
	delete(this->textShader);
	delete(this->pbrShader);

	delete(this->uiElement);

	delete(this->treeLevel);
	delete(this->torch);

	delete(this->gobbo);
	delete(this->gobboWalk);
	delete(this->gobboWalkAnimator);
	delete(this->gobboIdle);
	delete(this->gobboIdleAnimator);
	delete(this->gobboJump);
	delete(this->gobboJumpAnimator);

	delete(this->spiderMove);
	delete(this->spiderMoveAnimator);

	delete(this->testRenderer);
	delete(this->testTransform);
	delete(this->goblinRenderer);
	delete(this->goblinTransform);

	delete(this->backpack);
	delete(this->goblin);

	delete(this->goblinAnimController);
	delete(this->spiderAnimController);

	delete(this->camera);

	delete(this->noiseMaker);

	delete(this->particleGenerator);

	delete(this->boxCollider);
	delete(this->boxCollider2);
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
}

void RenderingSystem::Update()
{
	// Clear previous frame.
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update camera.
	this->camera->Update();

	// Define MVP matrices.
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	// Lighting data.
	glm::vec3 lightDir = glm::normalize(glm::vec3(0.0f, -1.0f, 1.0f));
	glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 5.0f);
	glm::vec3 lightAmbient = glm::vec3(0.15f, 0.15f, 0.15f);
	glm::vec3 lightDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	glm::vec3 lightSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

	glm::vec3 pointLightPos = glm::vec3(-45.0f, 20.0f, -89.0f);
	glm::vec3 pointLightAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 pointLightDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 pointLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	float pointLightIntensity = this->noiseMaker->Noise(glfwGetTime());

	// Draw goblin.
	this->goblin->GetRenderer()->AnimUpdate(projection,
										    view,
											camera->position,
											lightDir,
											lightAmbient,
											lightDiffuse,
											lightSpecular,
											this->goblinAnimController);

	// Draw spider.
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.0f, 0.0f, 150.0f));
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
	this->standardShader->SetVec3("pointLight.position", pointLightPos + glm::vec3(0.0f, 5.0f, 0.0f));
	this->standardShader->SetVec3("pointLight.ambient", pointLightAmbient);
	this->standardShader->SetVec3("pointLight.diffuse", pointLightDiffuse);
	this->standardShader->SetVec3("pointLight.specular", pointLightSpecular);
	this->standardShader->SetFloat("pointLight.constant", 1.0f);
	this->standardShader->SetFloat("pointLight.linear", 0.1f);
	this->standardShader->SetFloat("pointLight.quadratic", 0.001f);
	this->standardShader->SetFloat("pointLight.intensity", pointLightIntensity);

	this->treeLevel->Draw(*this->standardShader);

	// TODO Volumetric lighting test.
	/*
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
	*/

	// Grass.
	this->grassShader->Use();
	this->grassShader->SetMat4("projection", projection);
	this->grassShader->SetMat4("view", view);
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
	glBindTexture(GL_TEXTURE_2D, this->particleTexture);

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
	/*
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
	*/

	// Text.
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(this->windowWidth), 0.0f, static_cast<float>(this->windowHeight));
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	this->textShader->Use();
	this->textShader->SetMat4("projection", orthoProjection);
	this->textShader->SetMat4("model", model);
	this->uiElement->RenderText(this->textShader, "X", 100.0f, 100.0f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	*/

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
	glDepthFunc(GL_LEQUAL);
	this->skyboxShader->Use();
	this->skyboxShader->SetMat4("view", glm::mat3(view));
	this->skyboxShader->SetMat4("projection", projection);
	glBindVertexArray(this->skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

	// Swap to next render buffer for next frame.
	glfwSwapBuffers(window->GetGLFWWindow());
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