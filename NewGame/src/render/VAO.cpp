#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &id);

    this->animated = false;

    //std::cout << "VAO: " << &id << std::endl;
}

void VAO::Bind()
{
	glBindVertexArray(id);
}

void VAO::SetUp()
{
    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    if (animated)
    {
        // Bone Ids
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_INT, GL_FALSE, (sizeof(Vertex) + sizeof(Bone)), (void*)(sizeof(Vertex) + offsetof(Bone, boneId)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, (sizeof(Vertex) + sizeof(Bone)), (void*)(sizeof(Vertex) + offsetof(Bone, weight)));
    }

    /*
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    */

    glBindVertexArray(0);
}