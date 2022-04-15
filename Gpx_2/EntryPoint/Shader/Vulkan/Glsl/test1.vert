
#version 450

layout(location = 0)
out vec3 Color_Passdown;

vec2 Pre_Position[3] =
    vec2[](
        vec2( 0.0, -0.5),
        vec2( 0.5,  0.5),
        vec2(-0.5,  0.5)
    );

vec3 Pre_Color[3] =
    vec3[](
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
    );

void main()
{
    gl_Position = vec4(Pre_Position[gl_VertexIndex], 0.0, 1.0);
    Color_Passdown = Pre_Color[gl_VertexIndex];
}
