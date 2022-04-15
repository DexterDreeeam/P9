
#version 450

//==========================================

layout(location = 0)
in vec3 in_Position;

layout(location = 1)
in vec2 in_TextureCoord;

//==========================================

layout(location = 0)
out vec2 out_TextureCoord;

//==========================================

layout(binding = 0)
uniform _uni_Tsf
{
    mat4 tsf;
} uni_Tsf;

//==========================================

void main()
{
    vec4 pos = vec4(in_Position, 1.0);
    gl_Position = uni_Tsf.tsf * pos;
    out_TextureCoord = in_TextureCoord;
}
