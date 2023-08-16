#pragma once

#include <glm/glm.hpp>

#include "../ecs/Component.h"
#include "../ecs/Transform.h"
#include "Shader.h"
#include "Model.h"
#include "../util/Time.h"
#include "Animator.h"
#include "../anim/AnimationController.h"

class MeshRenderer : public Component
{
public:

	Shader* shader;
	Model* model;

	MeshRenderer(Shader* shader, Model* model);

	void Update(glm::mat4 projection, glm::mat4 view);
	void AnimUpdate(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos, 
					glm::vec3 lightDir, glm::vec3 lightAmbient, glm::vec3 lightDiffuse, 
					glm::vec3 lightSpecular, AnimationController* controller, glm::vec3 pointLightPos, 
					glm::vec3 pointLightAmbient, glm::vec3 poinLightDiffuse,
					glm::vec3 pointLightSpecular, float pointLightInstensity);
};