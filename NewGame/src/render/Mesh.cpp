#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::vector<Bone> bones)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->bones = bones;

    SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
    // bind appropriate textures.
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        
        // Retrieve texture number (the N in diffuse_textureN).
        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }

    // Draw mesh
    glBindVertexArray(vaoTest->id);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Unbind texture.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
    // Create buffers and array.
    vaoTest = new VAO();
    vboTest = new VBO();
    eboTest = new EBO();

    vaoTest->Bind();

    // Pass vertex and index data to buffers.
    vboTest->AddVertexData(vertices);
    eboTest->AddIndexData(indices);

    // Setup the vertex attribute data.
    vaoTest->SetUp();
}