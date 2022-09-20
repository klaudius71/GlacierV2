#version 460

layout (location = 0) in vec3 position;

layout (std140, binding = 0) uniform Matrices
{
    mat4 proj_matrix;
    mat4 view_matrix;
};

uniform mat4 world_matrix;
uniform vec4 color;

out vec4 col;

void main()
{
    gl_Position = proj_matrix * view_matrix * world_matrix * vec4(position, 1.0f);
    col = color;
}
