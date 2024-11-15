#version 460

layout (location = 0) in  vec2 in_position;
layout (location = 1) in  vec2 in_uv;

layout (location = 0) out vec2 out_uv;

layout (location = 0) uniform mat4 u_model;
layout (location = 1) uniform mat4 u_view;
layout (location = 2) uniform mat4 u_projection;

void main()
{
    out_uv      = in_uv;
    gl_Position = u_projection * u_view * u_model * vec4(in_position, 0.0, 1.0);
}