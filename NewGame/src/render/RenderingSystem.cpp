#include "RenderingSystem.h"

RenderingSystem::RenderingSystem(Window *window)
{
	glEnable(GL_DEPTH_TEST);

	this->window = window;
	this->windowWidth = window->GetWidth();
	this->windowHeight = window->GetHeight();

	this->ourShader = new Shader("res/shaders/model_loading.vs", "res/shaders/model_loading.fs");
	this->standardShader = new Shader("res/shaders/standard.vs", "res/shaders/standard.fs");
	this->animShader = new Shader("res/shaders/anim.vs", "res/shaders/anim.fs");
	this->skyboxShader = new Shader("res/shaders/skybox.vs", "res/shaders/skybox.fs");
	this->blendShader = new Shader("res/shaders/blending.vs", "res/shaders/blending.fs");

	//this->ourModel = new Model("res/obj/backpack.obj");
	//this->modelTwo = new Model(CubeVertices::GetVertices(), CubeVertices::GetIndices(), "brickwall.jpg", "res/textures/", "texture_diffuse");
	this->peachCastle = new Model("res/obj/peach_castle/peach_castle.obj");

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

	/*
	// Setup backpack.
	// TODO much of this should be moved to entity/component creation.
	this->backpack = new Entity("backpack");
	this->backpack->components.reserve(2);

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, -10.0f);
	glm::vec3 rot = glm::vec3(0.0f, 1.571f, 0.0f);
	glm::vec3 sca = glm::vec3(1.0f, 1.0f, 1.0f);
	this->testTransform = new Transform(this->backpack, pos, rot, sca);
	this->testTransform->entity = this->backpack;
	this->backpack->components.push_back(this->testTransform);

	this->testRenderer = new MeshRenderer(this->ourShader, this->ourModel);
	this->testRenderer->entity = this->backpack;
	this->backpack->components.push_back(this->testRenderer);
	*/

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
}

RenderingSystem::~RenderingSystem()
{
	delete(this->ourShader);
	delete(this->standardShader);
	delete(this->animShader);
	delete(this->skyboxShader);
	delete(this->blendShader);

	delete(this->ourModel);
	delete(this->modelTwo);
	delete(this->peachCastle);

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
	glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 5.0f);
	glm::vec3 lightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 lightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightSpecular = glm::vec3(0.3f, 0.3f, 0.3f);

	/*
	// Draw backpack.
	//this->backpack->GetRenderer()->Update(projection, view);
	
	// Draw cube.
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, -10.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	standardShader->Use();
	standardShader->SetMat4("model", model);
	standardShader->SetMat4("view", view);
	standardShader->SetMat4("projection", projection);
	standardShader->SetVec3("viewPos", camera->position);
	standardShader->SetFloat("material.shininess", 32.0);
	standardShader->SetInt("material.diffuse", 0);
	standardShader->SetInt("material.specular", 0);
	standardShader->SetVec3("light.position", lightPos);
	standardShader->SetVec3("light.ambient", lightAmbient);
	standardShader->SetVec3("light.diffuse", lightDiffuse);
	standardShader->SetVec3("light.specular", lightSpecular);
	
	modelTwo->Draw(*standardShader);
	*/

	// Draw goblin.
	this->goblin->GetRenderer()->AnimUpdate(projection,
										    view,
											camera->position,
											lightPos,
											lightAmbient,
											lightDiffuse,
											lightSpecular,
											this->goblinAnimController);

	// Draw the peach castle.
	model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, -15.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	this->standardShader->Use();
	this->standardShader->SetMat4("model", model);
	this->standardShader->SetMat4("view", view);
	this->standardShader->SetMat4("projection", projection);
	this->standardShader->SetVec3("viewPos", camera->position);
	this->standardShader->SetFloat("material.shininess", 32.0);
	this->standardShader->SetInt("material.diffuse", 0);
	this->standardShader->SetInt("material.specular", 0);
	this->standardShader->SetVec3("light.position", lightPos);
	this->standardShader->SetVec3("light.ambient", lightAmbient);
	this->standardShader->SetVec3("light.diffuse", lightDiffuse);
	this->standardShader->SetVec3("light.specular", lightSpecular);

	this->peachCastle->Draw(*this->standardShader);

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

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

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