#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(glm::vec3 position, unsigned int numParticles, float life)
{
	this->numParticles = numParticles;
	this->time = Time::GetInstance();
	this->life = life;
	this->position = position;

	float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	// Create the particles.
	for (unsigned int i = 0; i < this->numParticles; i++)
	{
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		float zVelocity = 2.0f * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f);
		float yVelocity = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.5f) + 0.5f;

		glm::vec3 velocity = glm::vec3(0.0f, yVelocity, zVelocity);

		float life = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) + 0.5f) * 3.0f;

		this->particles.push_back(Particle(this->position, color, velocity, life));
	}
}

void ParticleGenerator::Update()
{
	for (unsigned int i = 0; i < this->numParticles; i++)
	{
		// Get a reference to the next particle in the list.
		Particle& p = this->particles[i];
		
		// Update the life spawn of the particle.
		p.life -= this->time->GetDeltaTime();

		// Update the particle or reset it.
		if (p.life > 0.0f)
		{
			p.position += p.velocity * this->time->GetDeltaTime();
			p.color.z -= this->time->GetDeltaTime() / 3.0f;
			p.color.y -= this->time->GetDeltaTime() / 3.0f;
			p.color.w -= this->time->GetDeltaTime() / 2.0f;
		}
		else
		{
			Respawn(i);
		}
	}
}

void ParticleGenerator::Respawn(unsigned int index)
{
	// Get reference to specified particle.
	Particle& p = this->particles[index];

	// Reset particle attributes.
	p.life = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) + 0.5f) * 3.0f;
	p.position = this->position;
	p.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}