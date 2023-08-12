#include "Particle.h"

Particle::Particle()
{
	this->position = glm::vec3(0.0f);
	this->velocity = glm::vec3(0.0f);
	this->color = glm::vec4(1.0f);
	this->life = 0.0f;
}

Particle::Particle(glm::vec3 position, glm::vec4 color, glm::vec3 velocity, float life)
{
	this->position = position;
	this->velocity = velocity;
	this->color = color;
	this->life = life;
}