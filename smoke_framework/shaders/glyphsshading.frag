#version 330 core
// glyphsshading fragment shader

in vec3 vertPosition;
in float value;

uniform sampler1D textureSampler;

out vec4 color;

const vec3 vertNormal = vec3(0.0F, 0.0F, 1.0F); // Up
const vec3 viewPosition = vec3(300.0F, 300.0F, 200.0F);
const vec3 lightPosition = vec3(300.0F, 300.0F, 200.0F);
const vec4 materialCoefficients = vec4(0.5F, 0.5F, 0.8F, 1.0F);

void main()
{    
    vec3 materialColor = texture(textureSampler, value).rgb;
    // Normalize

    float ka = materialCoefficients[0];
    float kd = materialCoefficients[1];
    float ks = materialCoefficients[2];
    float alpha = materialCoefficients[3];

    vec3 N = normalize(vertNormal);
    vec3 V = normalize(viewPosition);
    vec3 L = normalize(lightPosition);
    vec3 H = normalize(L + V);

    vec3 amb = ka * materialColor;
    vec3 dif = kd * materialColor * max(dot(L,N),0);
    vec3 spc = ks * materialColor * pow(max(dot(H, N), 0.0),alpha);

    vec3 shader;
    shader =  materialColor * amb ;
    shader += materialColor * dif;
    shader += materialColor * spc;


    color = vec4(shader, 1.0F);
}
