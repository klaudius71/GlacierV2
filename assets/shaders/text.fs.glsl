#version 460 core

uniform sampler2D bitmap;
uniform vec4 color;

in vec2 tex_coord;

out vec4 FragColor;

void main()
{
	FragColor = vec4(color.xyz, color.w * texture(bitmap, tex_coord).r);
}