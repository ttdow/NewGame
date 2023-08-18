#version 330 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec4 fragPosLightSpace;

uniform vec3 lightDir;

uniform sampler2D texture1;
uniform sampler2D shadowMap;

void main()
{
    vec4 texColor = texture(texture1, texCoord);

    if (texColor.a < 0.1)
    {
        discard;
    }

    // Shadows.
    vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w * 0.5) + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;

    fragColor = texColor * shadow;
}