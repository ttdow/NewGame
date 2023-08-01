#pragma once

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	//glm::vec3 color;
	glm::vec2 texCoords;
	//glm::vec3 tangent;
	//glm::vec3 biTangent;

	//int m_BoneIDs[MAX_BONE_INFLUENCE];
	//float m_Weights[MAX_BONE_INFLUENCE];
};