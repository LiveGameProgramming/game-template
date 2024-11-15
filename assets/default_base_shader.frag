#version 460

layout (location = 0) in  vec3 in_normal;

layout (location = 0) out vec4 out_color;

layout (binding = 1, std140) uniform u_material
{
    vec3 diffuse;
}
material;

layout (binding = 2, std140) uniform u_light
{
    vec3  direction;
    float ambient;
    vec3  color;
}
light;

void main()
{
    vec3 normal    = normalize(in_normal);
    vec3 direction = normalize(-light.direction);

    vec3 ambient_color = material.diffuse * light.color * light.ambient;
    vec3 diffuse_color = material.diffuse * light.color * max(dot(normal, direction), 0.1);

    vec3 color = ambient_color + diffuse_color;
     out_color = vec4(color, 1.0);
}