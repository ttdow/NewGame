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