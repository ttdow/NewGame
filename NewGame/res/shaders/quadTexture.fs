#version 330 core

out vec4 fragColor;

in vec2 texCoords;

// G-Buffer textures.
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

// Lights.
uniform vec3 lightPos[12];

// Camera.
uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraPos;

void main()
{
    // Same gBuffer textures.
    vec3 worldPos = texture(gPosition, texCoords).rgb;
    vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 albedo = texture(gAlbedo, texCoords).rgb;

    // Prep lighting variables.
    vec3 result = albedo * 0.1; // Ambient
    vec3 viewDir = normalize(cameraPos - worldPos);

    // Calculate lighting for each point light.
    for (int i = 0; i < 12; i++)
    {
        vec3 lightDir = normalize(lightPos[i] - worldPos);
        float distance = length(lightPos[i] - worldPos);
        float attenuation = 1.0 / (1.0f + 0.7f * distance + 1.8f * (distance * distance));
        vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo;
        result += attenuation * diffuse;
    }

    fragColor = vec4(result, 1.0);
}