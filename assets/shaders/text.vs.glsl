#version 420

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 proj_matrix;
uniform mat4 world_matrix;

uniform vec4 sprite_data; // vec2 texel_pos, vec2 size;

uniform sampler2D bitmap;

out vec2 tex_coord;

void main()
{
    const vec2 tex_size = textureSize(bitmap, 0);
    tex_coord = (sprite_data.xy / tex_size) + uv * vec2(sprite_data.zw / tex_size);
	gl_Position = proj_matrix * world_matrix * vec4(position, 1.0f);
}