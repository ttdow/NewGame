#pragma once

#include <glm/glm.hpp>

#include <iostream>

class BoxCollider
{
public:

	glm::vec3 min;
	glm::vec3 max;

	glm::vec3 center;
	glm::vec3 extents;

	glm::mat3 orientation;

	glm::vec3 color;
	unsigned int vbo, vao;

	float vertices[36 * 3] =
	{
		/// Position		
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	BoxCollider()
	{
		this->min = glm::vec3(0.0f, 0.0f, 0.0f);
		this->max = glm::vec3(1.0f, 1.0f, 1.0f);
		this->color = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	void CheckCollision(BoxCollider* other)
	{
		// Check if box A's right edge is to the right of box B's left edge.
		if ((this->center.x + this->extents.x) >= (other->center.x - other->extents.x))
		{
			// Check if box A's left edge is to the left of box B's right edge.
			if ((this->center.x - this->extents.x) <= (other->center.x + other->extents.x))
			{
				// Check if box A's bottom edge is below box box B's top edge.
				if ((this->center.y - this->extents.y) <= (other->center.y + other->extents.y))
				{
					// Check if box A's top edge is above box B's bottom edge.
					if ((this->center.y + this->extents.y) >= (other->center.y - other->extents.y))
					{
						// Check if box A's front edge is in front of box B's back edge.
						if ((this->center.z - this->extents.z) <= (other->center.z + other->extents.z))
						{
							// Check if box A's back edge is behind of box B's front edge.
							if ((this->center.z + this->extents.z) >= (other->center.z - other->extents.z))
							{
							std::cout << "Collision detected!" << std::endl;
							}
						}
					}
				}
			}
		}
	}

	bool TestCollision(BoxCollider* other);
};