#version 420

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in uint tex_id;

layout (std140, binding = 0) uniform Matrices
{
    mat4 proj_matrix;
    mat4 view_matrix;
};

uniform mat4 world_matrix;

out VS_OUT
{
    vec2 tex_coord;
    flat uint tex_id;
} vs_out;

void main()
{
    gl_Position = proj_matrix * view_matrix * world_matrix * vec4(position, 1.0f);
    vs_out.tex_coord = uv;
    vs_out.tex_id = tex_id;
}
