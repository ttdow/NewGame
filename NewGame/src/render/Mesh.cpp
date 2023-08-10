#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, bool animated)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->animated = animated;

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
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);

        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }

    // Draw call for this mesh.
    glBindVertexArray(this->vao->id);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Unbind texture.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
    // Create buffers and array.
    this->vao = new VAO();
    this->vbo = new VBO();
    this->ebo = new EBO();

    this->vao->animated = this->animated;

    this->vao->Bind();

    // Pass vertex and index data to buffers.
    this->vbo->AddVertexData(vertices);
    this->ebo->AddIndexData(indices);

    // Setup the vertex attribute data.
    this->vao->Setup();
}