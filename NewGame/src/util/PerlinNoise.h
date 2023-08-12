#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <numeric>

class PerlinNoise
{
private:
	
	static PerlinNoise* instance;

	PerlinNoise(const PerlinNoise&) = delete;
	PerlinNoise& operator=(const PerlinNoise&) = delete;

	std::vector<int> permutation;

	float Fade(float t) const;
	float Lerp(float t, float a, float b) const;
	float Grad(int hash, float x) const;

public:

	static PerlinNoise* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new PerlinNoise();
		}

		return instance;
	}

	PerlinNoise();
	void SetRandomSeed(int seed);
	float Noise(float x) const;
};