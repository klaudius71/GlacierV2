#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 proj_matrix;
uniform mat4 world_matrix;

uniform vec4 sprite_data; // vec2 texel_origin, vec2 size

uniform sampler2D sprite_texture;

out vec2 tex_coord;

void main()
{
    const vec2 tex_size = textureSize(sprite_texture, 0);
    tex_coord = (sprite_data.xy / tex_size) + uv * vec2(sprite_data.zw / tex_size);

    mat4 world_matrix_shifted = world_matrix;
    world_matrix_shifted[0] *= sprite_data.z;
    world_matrix_shifted[1] *= sprite_data.w;
    gl_Position = proj_matrix * world_matrix_shifted * vec4(position, 1.0);
}