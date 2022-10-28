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

    // Transform
    vec3 viewPos = vec3(0);
    vec3 vertexPos = vec3(viewTransform * vec4(vertCoordinates_in,height,1));
    vec3 lightPos = vec3(viewTransform * vec4(lightPosition,1));

    // Phong
    vec3 N = normalize(normalTransform * vertNormals_in);
    vec3 L = normalize(lightPos - vertexPos);
    vec3 V = normalize(viewPos - vertexPos);

    // Reflection
    vec3 R = 2*dot(L,N)*N-L;

    shading = material[0] +  material[1] * max(dot(N,L),0) + material[2] * pow(max(dot(R,V),0),material[3]);

    // Hight change
    heightChange = abs(vertNormals_in[0])+abs(vertNormals_in[1]);
    heightChange = clamp(heightChange, 0, 1);

    // Clamp values.
    value = clamp(value_in, clampMin, clampMax);
    // Map the range [clampMin, clampMax] to [0, 1].
    value = (value-clampMin)/(clampMax-clampMin);
    value = pow(value, transferK);
}
