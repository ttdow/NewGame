#pragma once

#include <glm/glm.hpp>

#include "../ecs/Component.h"
#include "../ecs/Transform.h"
#include "Shader.h"
#include "Model.h"

class MeshRenderer : public Component
{
public:

	Shader* shader;
	Model* model;

	MeshRenderer(Shader* shader, Model* model);

	void Update(glm::mat4 projection, glm::mat4 view);
};