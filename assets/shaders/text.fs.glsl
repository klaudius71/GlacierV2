#version 460 core

uniform sampler2D bitmap;

in vec2 tex_coord;

out vec4 FragColor;

void main()
{
	FragColor = texture(bitmap, tex_coord);
}