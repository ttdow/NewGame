#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D hdrBuffer;
uniform sampler2D bloomBlur;

uniform bool hdr;
uniform bool bloom;
uniform float exposure;
uniform bool secondGamma;

void main()
{
    // Sample textures.
    vec3 result = texture(hdrBuffer, texCoords).rgb;

    // Add blurred bright areas for bloom effect.
    if (bloom)
    {
        vec3 bloomColor = texture(bloomBlur, texCoords).rgb;
        result += bloomColor;
    }

    // Perform HDR tone mapping.
    if (hdr)
    {
        result = vec3(1.0) - exp(-result * exposure);
    }

    // Gamma correction from linear color space for sRGB output.
    if (secondGamma)
    {
        float inverseGamma = 1.0 / 2.2;
        result = pow(result, vec3(inverseGamma));
    }
    
    fragColor = vec4(result, 1.0);
}