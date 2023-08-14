#include "BoxCollider.h"

void CalculateInterval(BoxCollider& box, glm::vec3& axis, float& min, float& max)
{
	glm::vec3 rotatedAxis = box.orientation * axis;
	float dotCenter = glm::dot(rotatedAxis, box.center);
}

bool BoxCollider::TestCollision(BoxCollider* other)
{
	for (unsigned int i = 0; i < 3; i++)
	{
		glm::vec3 axis1 = this->orientation[i];
		float min1, max1, min2, max2;

	}
}