
#version 450

layout(location = 0)
in vec3 Color_Takeover;

layout(location = 0)
out vec4 Color_Printout;

void main()
{
    Color_Printout = vec4(Color_Takeover, 1.0);
}
