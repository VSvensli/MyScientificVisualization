#version 330 core
// Colormap fragment shader

in float value;

uniform vec3 colorMapColors[3];

out vec4 color;

void main()
{
    //vec3 colorFromColorMap = (colorMapColors[0] * value);
    int idx = int(step(0.333F,value) + step(0.667F,value));

    vec3 colorFromColorMap = colorMapColors[idx];

    color = vec4(colorFromColorMap, 1.0F);
}
