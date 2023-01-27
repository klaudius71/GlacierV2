#version 420

uniform vec4 color;
uniform sampler2D textures[4];

in VS_OUT
{
    vec2 tex_coord;
    flat uint tex_id;
} fs_in;

out vec4 FragColor;

void main()
{
    FragColor = texture(textures[fs_in.tex_id], fs_in.tex_coord) * color;
}
