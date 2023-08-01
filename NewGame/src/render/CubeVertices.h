#pragma once

#include "Vertex.h"

#include <vector>

class CubeVertices
{
private:

	static CubeVertices* instance;

	CubeVertices();

	CubeVertices(const CubeVertices&) = delete;
	CubeVertices& operator=(const CubeVertices&) = delete;

public:

	static CubeVertices* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new CubeVertices();
		}

		return instance;
	}

	static std::vector<Vertex> GetVertices();
	static std::vector<unsigned int> GetIndices();
};

/*
unsigned int LoadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
	if (data)
	{
		GLenum format;
		if (nChannels == 1) format = GL_RED;
		else if (nChannels == 3) format = GL_RGB;
		else if (nChannels == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}

float someVertices[] =
{
	// Position				// Normal				// Color			// Texture
	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.f,	0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.f,	1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.f,	0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, 1.f,	1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, 1.f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, 1.f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, 1.f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, 1.f,	0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, 1.f,	1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f, 1.f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f, 1.f,	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f, 1.f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f, 1.f,	0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f, 1.f,	0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f, 1.f,	1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f, 1.f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f, 1.f,	1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f, 1.f,	1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f, 1.f,	1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f, 1.f,	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f, 1.f,	0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f, 1.f,	0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f, 1.f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f, 1.f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f, 1.f,	1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f, 1.f,	0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f, 1.f,	0.0f, 1.0f
};


glm::vec3 cubePositions[] =
{
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] =
{
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

void OldStuff()
{
	unsigned int VBO, cubeVAO, EBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// UV
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);

	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// UV
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	unsigned int texture1, texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("res/textures/container.jpg", &width, &height, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("res/textures/awesomeface.png", &width, &height, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	unsigned int diffuseMap = LoadTexture("res/textures/container2.png");
	unsigned int specularMap = LoadTexture("res/textures/container2_specular.png");

	/*
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.ID, "texture1"), 0);
	lightingShader.SetInt("texture2", 1);

	lightCubeShader.Use();
	lightCubeShader.SetInt("texture1", 0);
	lightCubeShader.SetInt("texture2", 1);

	lightingShader.Use();
	lightingShader.SetInt("material.diffuse", 0);
	lightingShader.SetInt("material.specular", 1);
}

void OtherOldStuff()
{
	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	*/

	/*
	lightingShader.Use();
	//lightingShader.SetVec3("light.position", lightPos);
	//lightingShader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
	//lightingShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//lightingShader.SetVec3("lightPos", lightPos);
	lightingShader.SetVec3("viewPos", camera.position);

	//glm::vec3 lightColor;
	//lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
	//lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
	//lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
	//glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
	//lightingShader.SetVec3("light.ambient", ambientColor);
	//lightingShader.SetVec3("light.diffuse", diffuseColor);
	//lightingShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//lightingShader.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	//lightingShader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);

	//lightingShader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	//lightingShader.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
	//lightingShader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
	lightingShader.SetFloat("material.shininess", 32.0f);
	//lightingShader.SetFloat("material.shininess", 64.0f);

	// directional light
	lightingShader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	lightingShader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	lightingShader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	lightingShader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	lightingShader.SetVec3("pointLights[0].position", pointLightPositions[0]);
	lightingShader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	lightingShader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	lightingShader.SetFloat("pointLights[0].constant", 1.0f);
	lightingShader.SetFloat("pointLights[0].linear", 0.09f);
	lightingShader.SetFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	lightingShader.SetVec3("pointLights[1].position", pointLightPositions[1]);
	lightingShader.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	lightingShader.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	lightingShader.SetFloat("pointLights[1].constant", 1.0f);
	lightingShader.SetFloat("pointLights[1].linear", 0.09f);
	lightingShader.SetFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	lightingShader.SetVec3("pointLights[2].position", pointLightPositions[2]);
	lightingShader.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	lightingShader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	lightingShader.SetFloat("pointLights[2].constant", 1.0f);
	lightingShader.SetFloat("pointLights[2].linear", 0.09f);
	lightingShader.SetFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	lightingShader.SetVec3("pointLights[3].position", pointLightPositions[3]);
	lightingShader.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	lightingShader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	lightingShader.SetFloat("pointLights[3].constant", 1.0f);
	lightingShader.SetFloat("pointLights[3].linear", 0.09f);
	lightingShader.SetFloat("pointLights[3].quadratic", 0.032f);
	// spotLight
	lightingShader.SetVec3("spotLight.position", camera.position);
	lightingShader.SetVec3("spotLight.direction", camera.front);
	lightingShader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	lightingShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	lightingShader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	lightingShader.SetFloat("spotLight.constant", 1.0f);
	lightingShader.SetFloat("spotLight.linear", 0.09f);
	lightingShader.SetFloat("spotLight.quadratic", 0.032f);
	lightingShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	lightingShader.SetMat4("projection", projection);

	glm::mat4 view = camera.GetViewMatrix();
	lightingShader.SetMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	lightingShader.SetMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	glBindVertexArray(cubeVAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader.SetMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	lightCubeShader.Use();
	lightCubeShader.SetMat4("projection", projection);
	lightCubeShader.SetMat4("view", view);

	/*
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	lightCubeShader.SetMat4("model", model);
	*/

	/*
	glBindVertexArray(lightCubeVAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	for (unsigned int i = 0; i < 4; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		lightCubeShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	/*
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteVertexArrays(1, &lightCubeVAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
}
*/