#version 460 core

#define MAX_CHARACTERS 100

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 proj_matrix;
uniform mat4 world_matrix;

uniform vec4 glyph_data[MAX_CHARACTERS]; // vec2 texel_pos, vec2 size
uniform vec4 world_data[MAX_CHARACTERS]; // vec2 screen_pos, vec2 scale

uniform sampler2D bitmap;

out vec2 tex_coord;

void main()
{
    const vec2 tex_size = textureSize(bitmap, 0);
    tex_coord = (glyph_data[gl_InstanceID].xy / tex_size) + uv * vec2(glyph_data[gl_InstanceID].zw / tex_size);

    mat4 world_matrix_shifted = world_matrix;
    world_matrix_shifted[0] *= world_data[gl_InstanceID].z;
    world_matrix_shifted[1] *= world_data[gl_InstanceID].w;
    world_matrix_shifted[3] += world_matrix * vec4(world_data[gl_InstanceID].xy, 0.0, 0.0);

	gl_Position = proj_matrix * world_matrix_shifted * vec4(position, 1.0f);
}