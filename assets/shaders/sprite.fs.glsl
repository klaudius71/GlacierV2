#version 460 core

uniform sampler2D sprite_texture;

in vec2 tex_coord;

out vec4 FragColor;

void main()
{
    FragColor = texture(sprite_texture, tex_coord);
}