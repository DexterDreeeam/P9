
#version 450

//==========================================

layout(location = 0)
in vec2 in_TextureCoord;

//==========================================

layout(location = 0)
out vec4 out_Color;

//==========================================

layout(binding = 1)
uniform sampler2D sampler_Texture;

//==========================================

void main()
{
    out_Color = texture(sampler_Texture, in_TextureCoord);
}
