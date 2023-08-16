#include "Skybox.h"

Skybox::Skybox()
{
	glGenVertexArrays(1, &this->skyboxVAO);
	glGenBuffers(1, &this->skyboxVBO);
	glBindVertexArray(this->skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->skyboxVertices), &this->skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	this->skyboxShader = new Shader("res/shaders/skybox.vs", "res/shaders/skybox.fs");

	this->skyboxShader->Use();
	this->skyboxShader->SetInt("skybox", 0);

	std::vector<std::string> cubeMapTextures;
	cubeMapTextures.push_back("res/textures/skybox/right.jpg");
	cubeMapTextures.push_back("res/textures/skybox/left.jpg");
	cubeMapTextures.push_back("res/textures/skybox/top.jpg");
	cubeMapTextures.push_back("res/textures/skybox/bottom.jpg");
	cubeMapTextures.push_back("res/textures/skybox/front.jpg");
	cubeMapTextures.push_back("res/textures/skybox/back.jpg");

	glGenTextures(1, &this->cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapTexture);

	int width, height, channels;
	unsigned char* data;
	for (unsigned int i = 0; i < cubeMapTextures.size(); i++)
	{
		stbi_set_flip_vertically_on_load(false);
		data = stbi_load(cubeMapTextures[i].c_str(), &width, &height, &channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << cubeMapTextures[i] << std::endl;
		}

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Skybox::~Skybox()
{
	delete(this->skyboxShader);
}

void Skybox::Draw(glm::mat4 view, glm::mat4 projection)
{
	// Set depth function to less than or equal.
	glDepthFunc(GL_LEQUAL);

	this->skyboxShader->Use();
	this->skyboxShader->SetMat4("view", glm::mat3(view));
	this->skyboxShader->SetMat4("projection", projection);

	// Bind skybox VAO data.
	glBindVertexArray(this->skyboxVAO);

	// Activate skybox cubemap texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapTexture);

	// Draw the skybox.
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	// Unbind the skybox VAO data.
	glBindVertexArray(0);

	// Set depth function back to less than.
	glDepthFunc(GL_LESS);
}