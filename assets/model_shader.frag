#version 460

layout (location = 0) in  vec3 in_normal;
layout (location = 1) in  vec2 in_uv;

layout (location = 0) out vec4 out_color;

layout (binding = 0)  uniform sampler2D u_diffuse_texture;

void main()
{
    vec3 color = texture(u_diffuse_texture, in_uv).rgb;
     out_color = vec4(color, 1.0);
}