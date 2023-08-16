#include "MeshRenderer.h"

#include "../ecs/Entity.h"

MeshRenderer::MeshRenderer(Shader* shader, Model* model)
{
	this->entity = nullptr;

	this->shader = shader;
	this->model = model;
}

void MeshRenderer::Update(glm::mat4 projection, glm::mat4 view)
{
	this->shader->Use();
	this->shader->SetMat4("model", this->entity->GetTransform()->modelMatrix);
	this->shader->SetMat4("view", view);
	this->shader->SetMat4("projection", projection);
	this->model->Draw(*this->shader);
}

void MeshRenderer::AnimUpdate(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos, 
							  glm::vec3 lightDir, glm::vec3 lightAmbient, glm::vec3 lightDiffuse, 
							  glm::vec3 lightSpecular, AnimationController* controller,
							  glm::vec3 pointLightPos, glm::vec3 pointLightAmbient, glm::vec3 pointLightDiffuse,
							  glm::vec3 pointLightSpecular, float pointLightIntensity)
{
	this->shader->Use();
	this->shader->SetMat4("model", this->entity->GetTransform()->modelMatrix);
	this->shader->SetMat4("view", view);
	this->shader->SetMat4("projection", projection);
	this->shader->SetVec3("viewPos", cameraPos);
	this->shader->SetFloat("material.shininess", 32.0);
	this->shader->SetInt("material.diffuse", 0);
	this->shader->SetInt("material.specular", 0);
	this->shader->SetVec3("directionalLight.direction", lightDir);
	this->shader->SetVec3("directionalLight.ambient", lightAmbient);
	this->shader->SetVec3("directionalLight.diffuse", lightDiffuse);
	this->shader->SetVec3("directionalLight.specular", lightSpecular);
	this->shader->SetVec3("pointLight.position", pointLightPos + glm::vec3(0.0f, 5.0f, 0.0f));
	this->shader->SetVec3("pointLight.ambient", pointLightAmbient);
	this->shader->SetVec3("pointLight.diffuse", pointLightDiffuse);
	this->shader->SetVec3("pointLight.specular", pointLightSpecular);
	this->shader->SetFloat("pointLight.constant", 1.0f);
	this->shader->SetFloat("pointLight.linear", 0.1f);
	this->shader->SetFloat("pointLight.quadratic", 0.001f);
	this->shader->SetFloat("pointLight.intensity", pointLightIntensity);

	controller->Update(this->shader);

	this->model->Draw(*shader);
}