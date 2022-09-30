#version 460 core

layout (location = 0) in vec3 position;

layout (std140, binding = 0) uniform Matrices
{
    mat4 proj_matrix;
    mat4 view_matrix;
};

out vec3 tex_coords;

void main()
{
    tex_coords = position;
    const vec4 proj_view_pos = proj_matrix * mat4(mat3(view_matrix)) * vec4(position, 1.0f);
    gl_Position = proj_view_pos.xyww;
}