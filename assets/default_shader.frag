#version 460

layout (binding = 1, std140) uniform u_material
{
    vec3 diffuse;
}
material;

layout (location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(material.diffuse, 1.0);
}