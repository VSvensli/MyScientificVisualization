#version 330 core
// Height plot clamp vertex shader

layout (location = 0) in vec2 vertCoordinates_in;
layout (location = 1) in float height;
layout (location = 2) in float value_in;
layout (location = 3) in vec3 vertNormals_in;

// height value
out float value;
// depicts the brightness of the color
out float shading;
// magnitude of the gradient
out float heightChange;

uniform float clampMin;
uniform float clampMax;
uniform float transferK;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat3 normalTransform;

uniform vec4 material; // Contains 4 floats, in order: k_a, k_d, k_s, alpha.
uniform vec3 lightPosition;

void main()
{
    gl_Position = viewTransform * projectionTransform * vec4(vertCoordinates_in, height, 1.0F);

    // Ambient
    float ambient = material[0];

    // Diffuse
    vec3 fragPos = vec3(vertCoordinates_in, height);
    vec3 lightDir = lightPosition-fragPos;
    lightDir = normalize(lightDir);

    vec3 norm = vertNormals_in*normalTransform;
    norm = normalize(norm);

    float diff = max(dot(lightDir,norm), 0.0) * material[1];

    // Spectral
    vec3 V = vec3(1,1,1);
    V = normalize(V);

    vec3 reflection = 2*dot(norm,lightDir)*norm- lightDir;
    reflection = normalize(reflection);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //vec3 specular = specularStrength * spec * lightColor;
    float spec = pow(max(dot(reflection,V),0),material[3]) * material[2];

    shading = transferK + ambient + diff + spec;

    // Height change
    heightChange = abs(vertNormals_in[0])+abs(vertNormals_in[1]);
    heightChange = clamp(heightChange, 0, 1);

    // Clamp values.
    value = clamp(value_in, clampMin, clampMax);

    // Map the range [clampMin, clampMax] to [0, 1].
    value = (value-clampMin)/(clampMax-clampMin);


    value = pow(value, transferK);
}
