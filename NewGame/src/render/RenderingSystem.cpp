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

	// Particles.
	this->particleGenerator = new ParticleGenerator(glm::vec3(-45.0f, 24.0f, -89.0f), 50, 2.0f);
	
	// Models.
	this->treeLevel = new Model("res/obj/tree/tree_level.obj");
	this->torch = new Model("res/obj/torch/torch.obj");

	stbi_set_flip_vertically_on_load(true);
	this->gobbo = new Model("res/obj/gobbo.fbx");
	this->goblinAnimController = new AnimationController();
	this->gobboWalk = new Animation("res/obj/gobbo_walk.fbx", this->gobbo);
	this->gobboWalkAnimator = new Animator(this->gobboWalk);
	this->gobboIdle = new Animation("res/obj/gobbo_idle.fbx", this->gobbo);
	this->gobboIdleAnimator = new Animator(this->gobboIdle);
	this->gobboAttack = new Animation("res/obj/gobbo_attack.fbx", this->gobbo);
	this->gobboAttackAnimator = new Animator(this->gobboAttack);

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
	this->vegetation.push_back(glm::vec3(-1.5f, 0.0f, 1.00f));
	this->vegetation.push_back(glm::vec3(1.5f, 0.0f, 1.51f));
	this->vegetation.push_back(glm::vec3(0.0f, 1.0f, 1.7f));
	this->vegetation.push_back(glm::vec3(-0.3f, 0.5f, 2.3f));
	this->vegetation.push_back(glm::vec3(0.5f, 0.0f, 0.6f));

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
	glBindVertexArray(0);

	this->grassTexture = TextureFromFile("grass.png", "res/textures/", "texture_diffuse");

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

	delete(this->ourModel);
	delete(this->modelTwo);
	delete(this->treeLevel);
	delete(this->torch);

	delete(this->gobbo);
	delete(this->gobboWalk);
	delete(this->gobboWalkAnimator);
	delete(this->gobboIdle);
	delete(this->gobboIdleAnimator);

	delete(this->testRenderer);
	delete(this->testTransform);
	delete(this->goblinRenderer);
	delete(this->goblinTransform);

	delete(this->backpack);
	delete(this->goblin);

	delete(this->goblinAnimController);

	delete(this->camera);

	delete(this->noiseMaker);

	delete(this->particleGenerator);

	delete(this->boxCollider);
	delete(this->boxCollider2);
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
	glm::vec3 lightAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 lightDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	glm::vec3 lightSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

	// Draw goblin.
	this->goblin->GetRenderer()->AnimUpdate(projection,
										    view,
											camera->position,
											lightDir,
											lightAmbient,
											lightDiffuse,
											lightSpecular,
											this->goblinAnimController);
	
	glm::vec3 pointLightPos = glm::vec3(-45.0f, 20.0f, -89.0f);
	glm::vec3 pointLightAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 pointLightDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 pointLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	float pointLightIntensity = this->noiseMaker->Noise(glfwGetTime());

	/*
	// Draw goblin clone.
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glm::mat4 goblinCloneModel = model;
	this->standardShader->Use();
	this->standardShader->SetMat4("model", this->goblinTransform->modelMatrix);
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

	this->gobbo->Draw(*this->standardShader);
	*/

	/*
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
	*/

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

	/*
	// Grass.
	this->blendShader->Use();
	this->blendShader->SetMat4("projection", projection);
	this->blendShader->SetMat4("view", view);
	glBindVertexArray(this->blendVAO);

	glBindTexture(GL_TEXTURE_2D, this->grassTexture);

	for (unsigned int i = 0; i < this->vegetation.size(); i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, this->vegetation[i]);
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		this->blendShader->SetMat4("model", model);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	*/

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
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(this->goblinTransform->modelMatrix[3][0], 
											this->goblinTransform->modelMatrix[3][1] + 5.0f, 
										    this->goblinTransform->modelMatrix[3][2] + 2.0f));
	this->boxCollider->center = 

	this->colliderShader->Use();
	this->colliderShader->SetMat4("model", model);
	this->colliderShader->SetMat4("view", view);
	this->colliderShader->SetMat4("projection", projection);
	this->colliderShader->SetVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
	glBindVertexArray(this->boxCollider->vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// Collider 2 visualization.
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 5.0f, 2.0f));
	this->colliderShader->Use();
	this->colliderShader->SetMat4("model", model);
	this->colliderShader->SetMat4("view", view);
	this->colliderShader->SetMat4("projection", projection);
	this->colliderShader->SetVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
	glBindVertexArray(this->boxCollider->vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

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