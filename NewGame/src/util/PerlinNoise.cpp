#include "PerlinNoise.h"

PerlinNoise* PerlinNoise::instance = nullptr;

PerlinNoise::PerlinNoise()
{
	// Nothing.
}

void PerlinNoise::SetRandomSeed(int seed)
{
	this->permutation.resize(256);
	std::iota(this->permutation.begin(), this->permutation.end(), 0);
	std::default_random_engine engine(seed);
	std::shuffle(this->permutation.begin(), this->permutation.end(), engine);
	this->permutation.insert(this->permutation.end(), this->permutation.begin(), this->permutation.end());
}

float PerlinNoise::Noise(float x) const
{
	int i = static_cast<int>(std::floor(x)) & 255;
	x -= std::floor(x);
	float u = Fade(x);

	return Lerp(u, Grad(permutation[i], x), Grad(permutation[i + 1], x - 1));
}

float PerlinNoise::Fade(float t) const
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::Lerp(float t, float a, float b) const
{
	return a + t * (b - a);
}

float PerlinNoise::Grad(int hash, float x) const
{
	int h = hash & 15;
	float grad = 1.0 + (h & 7);
	
	if (h & 8) grad = -grad;

	float out = (h & 1) ? -grad : grad;
	float normalized = ((out / 8.0f) + 1) * 0.5f;

	return normalized;
}