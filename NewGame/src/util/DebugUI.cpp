#include "DebugUI.h"

DebugUI::DebugUI(GLFWwindow* window)
{
	this->window = window;
}

void DebugUI::Setup()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void DebugUI::NewFrame(RenderingSystem* renderingSystem)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//bool showDemoWindow = true;
	//ImGui::ShowDemoWindow(&showDemoWindow);

	ImGui::Begin("Rendering System");

	ImGui::Checkbox("Use G-Buffer", &renderingSystem->useGBuffer);

	ImGui::Checkbox("First Gamma", &renderingSystem->firstGamma);
	ImGui::Checkbox("Second Gamma", &renderingSystem->secondGamma);
	ImGui::Checkbox("HDR", &renderingSystem->hdr);
	ImGui::Checkbox("Bloom", &renderingSystem->bloom);

	if (renderingSystem->debugNormalMap)
	{
		renderingSystem->debugVertexNormals = false;
	}

	ImGui::Checkbox("Debug Vertex Normals", &renderingSystem->debugVertexNormals);

	if (renderingSystem->debugVertexNormals)
	{
		renderingSystem->debugNormalMap = false;
	}

	ImGui::Checkbox("Debug Normal Map", &renderingSystem->debugNormalMap);

	ImGui::InputFloat("bloomBrightness", &renderingSystem->bloomBrightness, 0.1f, 1.0f);
	ImGui::InputFloat("exposure", &renderingSystem->exposure, 0.1f, 1.0f);
	ImGui::InputFloat("shininess", &renderingSystem->shininess, 1.0f, 5.0f);

	ImGui::Text("Directional Light");
	glm::vec3& lightDir = renderingSystem->dirLight->direction;
	ImGui::InputFloat("lightDir.x", &lightDir.x, 0.1f, 1.0f);
	ImGui::InputFloat("lightDir.y", &lightDir.y, 0.1f, 1.0f);
	ImGui::InputFloat("lightDir.z", &lightDir.z, 0.1f, 1.0f);

	glm::vec3& ambientIntensity = renderingSystem->dirLight->ambientIntensity;
	ImGui::InputFloat("directionalLight.ambientIntensity", &ambientIntensity.x, 0.1f, 1.0f);
	ambientIntensity.y = ambientIntensity.x;
	ambientIntensity.z = ambientIntensity.x;

	glm::vec3& diffuseIntensity = renderingSystem->dirLight->diffuseIntensity;
	ImGui::InputFloat("directionalLight.diffuseIntensity", &diffuseIntensity.x, 0.1f, 1.0f);
	diffuseIntensity.y = diffuseIntensity.x;
	diffuseIntensity.z = diffuseIntensity.x;

	glm::vec3& specularIntensity = renderingSystem->dirLight->specularIntensity;
	ImGui::InputFloat("directionalLight.specularIntensity", &specularIntensity.x, 0.1f, 1.0f);
	specularIntensity.y = specularIntensity.x;
	specularIntensity.z = specularIntensity.x;

	ImGui::Text("Point Light");
	glm::vec3& pointLightPos = renderingSystem->pointLight->position;
	ImGui::InputFloat("pointLightPos.x", &pointLightPos.x, 0.1f, 1.0f);
	ImGui::InputFloat("pointLightPos.y", &pointLightPos.y, 0.1f, 1.0f);
	ImGui::InputFloat("pointLightPos.z", &pointLightPos.z, 0.1f, 1.0f);

	glm::vec3& pointAmbient = renderingSystem->pointLight->ambientIntensity;
	ImGui::InputFloat("pointLight.ambientIntensity", &pointAmbient.x, 0.1f, 1.0f);
	pointAmbient.y = pointAmbient.x;
	pointAmbient.z = pointAmbient.x;

	glm::vec3& pointDiffuse = renderingSystem->pointLight->diffuseIntensity;
	ImGui::InputFloat("pointLight.diffuseIntensity", &pointDiffuse.x, 0.1f, 1.0f);
	pointDiffuse.y = pointDiffuse.x;
	pointDiffuse.z = pointDiffuse.x;

	glm::vec3& pointSpecular = renderingSystem->pointLight->specularIntensity;
	ImGui::InputFloat("pointLight.specularIntensity", &pointSpecular.x, 0.1f, 1.0f);
	pointSpecular.y = pointSpecular.x;
	pointSpecular.z = pointSpecular.x;

	float& constant = renderingSystem->pointLight->constant;
	ImGui::InputFloat("pointLight.constant", &constant, 0.01f, 0.1f);
	
	float& linear = renderingSystem->pointLight->linear;
	ImGui::InputFloat("pointLight.linear", &linear, 0.01f, 0.1f);

	float& quadratic = renderingSystem->pointLight->quadratic;
	ImGui::InputFloat("pointLight.quadratic", &quadratic, 0.01f, 0.1f);

	ImGui::Text("Shadows");
	ImGui::Checkbox("Show Debug Shadow Map", &renderingSystem->showDebugShadowMap);

	glm::vec3& lightPos = renderingSystem->lightPos;
	ImGui::InputFloat("lightPos.x", &lightPos.x, 1.0f, 10.0f);
	ImGui::InputFloat("lightPos.y", &lightPos.y, 1.0f, 10.0f);
	ImGui::InputFloat("lightPos.z", &lightPos.z, 1.0f, 10.0f);

	glm::vec4& orthoValues = renderingSystem->orthoValues;
	ImGui::InputFloat("Orthographic Left", &orthoValues.x, 0.1f, 1.0f);
	ImGui::InputFloat("Orthographic Right", &orthoValues.y, 0.1f, 1.0f);
	ImGui::InputFloat("Orthographic Bottom", &orthoValues.z, 0.1f, 1.0f);
	ImGui::InputFloat("Orthographic Top", &orthoValues.w, 0.1f, 1.0f);

	ImGui::InputFloat("nearPlane", &renderingSystem->nearPlane, 0.1f, 1.0f);
	ImGui::InputFloat("FarPlane", &renderingSystem->farPlane, 1.0f, 10.0f);

	glm::vec3& lightColor = renderingSystem->lightColor;
	ImGui::InputFloat("lightColor.x", &lightColor.x, 0.5f, 5.0f);
	ImGui::InputFloat("lightColor.y", &lightColor.y, 0.5f, 5.0f);
	ImGui::InputFloat("lightColor.z", &lightColor.z, 0.5f, 5.0f);

	ImGui::InputFloat("minBias", &renderingSystem->minBias, 0.0001f, 0.001f);

	ImGui::End();

	ImGui::Render();
}

void DebugUI::EndFrame()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugUI::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}