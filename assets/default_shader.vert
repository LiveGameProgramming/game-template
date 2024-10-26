#version 460

layout (location = 0) in vec3 in_position;

layout (location = 0) uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * vec4(in_position, 1.0);
}