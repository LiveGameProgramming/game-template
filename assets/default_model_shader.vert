#version 460

layout (location = 0) in  vec3 in_position;
layout (location = 1) in  vec3 in_normal;
layout (location = 2) in  vec2 in_uv;

layout (location = 0) out vec3 out_normal;
layout (location = 1) out vec2 out_uv;

layout (location = 0) uniform mat4    u_model;
layout (binding  = 0, std140) uniform u_camera
{
    mat4 view;
    mat4 projection;
}
camera;

void main()
{
    out_uv      = in_uv;
    out_normal  = mat3(u_model) * in_normal;

    gl_Position = camera.projection * camera.view * u_model * vec4(in_position, 1.0);
}