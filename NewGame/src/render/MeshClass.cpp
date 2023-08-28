#include "MeshClass.h"

MeshClass::MeshClass()
{
	this->material = nullptr;
	this->vao = nullptr;
	this->vbo = nullptr;
	this->ebo = nullptr;
}

void MeshClass::Setup()
{
	this->vao = new VAO();
	this->vbo = new VBO();
	this->ebo = new EBO();

	this->vao->animated = false;

	this->vao->Bind();

	this->vbo->AddVertexData(this->vertices);
	this->ebo->AddIndexData(this->indices);

	this->vao->Setup();

	this->vao->Unbind();
}

void MeshClass::Draw(Shader& shader)
{
	// TODO Ignore meshes without materials (atm this is glass).
	if (this->material == nullptr) return;

	// TODO Ignore dirt decal meshes for now.
	if (std::strcmp(this->material->name.c_str(), "dirt_decal") != 0 &&
		std::strcmp(this->material->name.c_str(), "dirt_decal.001") != 0)
	{
		TextureManager* textureManager = TextureManager::GetInstance();

		// Bind textures.
		unsigned int albedoId = textureManager->GetTextureId(this->material->albedoMap);
		glActiveTexture(GL_TEXTURE0);
		shader.SetInt("albedoMap", 0);
		glBindTexture(GL_TEXTURE_2D, albedoId);

		unsigned int normalId = textureManager->GetTextureId(this->material->normalMap);
		glActiveTexture(GL_TEXTURE1);
		shader.SetInt("normalMap", 1);
		glBindTexture(GL_TEXTURE_2D, normalId);

		/*
		unsigned int metallicId = textureManager->GetTextureId(this->material->metallicMap);
		glActiveTexture(GL_TEXTURE2);
		shader.SetInt("metallicMap", 2);
		glBindTexture(GL_TEXTURE_2D, metallicId);

		unsigned int roughnessId = textureManager->GetTextureId(this->material->roughnessMap);
		glActiveTexture(GL_TEXTURE3);
		shader.SetInt("roughnessMap", 3);
		glBindTexture(GL_TEXTURE_2D, roughnessId);
		*/

		// Draw call.
		this->vao->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(this->indices.size()), GL_UNSIGNED_INT, 0);
		this->vao->Unbind();

		// Unbind texture.
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}