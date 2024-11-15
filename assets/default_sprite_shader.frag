#version 460

layout (location = 0) in  vec2 in_uv;

layout (location = 0) out vec4 out_color;

layout (binding  = 0)  uniform sampler2D u_texture;

void main()
{
    vec3 color = texture(u_texture, in_uv).rgb;
     out_color = vec4(color, 1.0);
}