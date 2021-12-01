
#version 450

layout(location = 0)
in vec3 in_Position;

layout(location = 1)
in vec3 in_Color;

layout(location = 0)
out vec3 out_Color;

//==========================================

layout(binding = 0)
uniform _uni_Color
{
    vec3 v3;
} uni_Color;

//==========================================

void main()
{
    vec4 pos = vec4(in_Position, 1.0);
    gl_Position = pos;
    out_Color = uni_Color.v3;
}
