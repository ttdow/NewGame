#include "RenderingSystem.h"

RenderingSystem::RenderingSystem(Window *window)
{
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);

	this->window = window;
	this->windowWidth = window->GetWidth();
	this->windowHeight = window->GetHeight();

	this->camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	this->ourShader = new Shader("res/shaders/model_loading.vs", "res/shaders/model_loading.fs");
	this->standardShader = new Shader("res/shaders/standard.vs", "res/shaders/standard.fs");
	this->animShader = new Shader("res/shaders/anim.vs", "res/shaders/anim.fs");

	this->ourModel = new Model("res/obj/backpack.obj");
	this->modelTwo = new Model(CubeVertices::GetVertices(), CubeVertices::GetIndices(), "brickwall.jpg", "res/textures/", "texture_diffuse");
	this->animModel = new Model("res/obj/animcube.fbx");
	this->anim = new Animation("res/obj/animcube.fbx", this->animModel);

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
}

RenderingSystem::~RenderingSystem()
{
	delete(this->ourShader);
	delete(this->standardShader);

	delete(this->ourModel);
	delete(this->modelTwo);
	delete(this->animModel);
	delete(this->anim);

	delete(this->testRenderer);
	delete(this->testTransform);

	delete(this->backpack);
}

void RenderingSystem::Update()
{
	// Clear previous frame.
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Define MVP matrices.
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	// Draw backpack.
	this->backpack->GetRenderer()->Update(projection, view);
	
	// Draw cube.
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(5.0f, 0.0f, -10.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 lightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightSpecular = glm::vec3(0.3f, 0.3f, 0.3f);

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

	// Draw animating cube model.
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -10.0f));
	model = model * this->anim->rotations[this->anim->GetAnimationFrame()];
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = model * this->anim->childTransformation;
	
	this->animShader->Use();
	this->animShader->SetMat4("model", model);
	this->animShader->SetMat4("view", view);
	this->animShader->SetMat4("projection", projection);
	this->animShader->SetVec3("viewPos", camera->position);
	this->animShader->SetFloat("material.shininess", 32.0);
	this->animShader->SetInt("material.diffuse", 0);
	this->animShader->SetInt("material.specular", 0);
	this->animShader->SetVec3("light.position", lightPos);
	this->animShader->SetVec3("light.ambient", lightAmbient);
	this->animShader->SetVec3("light.diffuse", lightDiffuse);
	this->animShader->SetVec3("light.specular", lightSpecular);

	this->animModel->Draw(*animShader);
	
	// Swap to next render buffer.
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