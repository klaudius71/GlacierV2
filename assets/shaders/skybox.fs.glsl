#version 460 core

uniform samplerCube skybox_tex;

in vec3 tex_coords;

out vec4 color;

void main()
{
	color = texture(skybox_tex, tex_coords);
}