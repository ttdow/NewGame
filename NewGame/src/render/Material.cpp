#include "Material.h"

Material::Material(std::string dir)
{
	this->albedo = new TextureClass(dir + "albedo.png", false);
	this->normal = new TextureClass(dir + "normal.png", false);
	this->metallic = new TextureClass(dir + "metallic.png", false);
	this->roughness = new TextureClass(dir + "roughness.png", false);
	this->ao = new TextureClass(dir + "ao.png", false);
}

void Material::BindMaterial()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->albedo->GetID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->normal->GetID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->metallic->GetID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->roughness->GetID());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, this->ao->GetID());
}