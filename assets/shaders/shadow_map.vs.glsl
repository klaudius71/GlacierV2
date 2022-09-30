#version 460 core

layout (location = 0) in vec3 position;

struct PhongADS
{
	vec3 ambient;
	vec3 diffuse;
	vec4 specular;
};

struct DirectionalLight
{
    PhongADS light_properties;
    vec3 direction; // world direction
};

layout (std140, binding = 2) uniform LightspaceMatrices
{
    mat4 lightspace_dir_light;
};

uniform mat4 world_matrix;

void main()
{
    gl_Position = lightspace_dir_light * world_matrix * vec4(position, 1.0f);
}