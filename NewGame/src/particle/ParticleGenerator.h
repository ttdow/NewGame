#pragma once

#include "Particle.h"
#include "../util/Time.h"

#include <vector>
#include <iostream>

class ParticleGenerator
{
public:

	std::vector<Particle> particles;

	ParticleGenerator(glm::vec3 position, unsigned int numParticles, float life);
	void Update();

private:

	Time* time;
	unsigned int numParticles;
	float life;
	glm::vec3 position;

	void Respawn(unsigned int index);
};