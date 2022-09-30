#version 420

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in uint tex_id;
layout (location = 3) in vec3 normal;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 bitangent;

layout (std140, binding = 0) uniform Matrices
{
    mat4 proj_matrix;
    mat4 view_matrix;
};
uniform mat4 world_matrix;

layout (std140, binding = 2) uniform LightspaceMatrices
{
	mat4 lightspace;
};

out VS_OUT
{
    vec4 position_cameraspace;
    vec4 position_lightspace;
    vec3 normal_cameraspace;
    vec2 tex_coord;
    flat uint tex_id;
} vs_out;

void main()
{
    const mat4 view_world = view_matrix * world_matrix;

    vs_out.position_cameraspace = view_world * vec4(position, 1.0f);
    gl_Position = proj_matrix * vs_out.position_cameraspace;
    vs_out.position_lightspace = lightspace * world_matrix * vec4(position, 1.0f);
    vs_out.normal_cameraspace = normalize(mat3(transpose(inverse(view_world))) // temporary (should be calculating this on CPU)
                                * normal);
    //vs_out.normal_cameraspace = normalize(mat3(view_world) * normal);
    vs_out.tex_coord = uv;
    vs_out.tex_id = tex_id;
}
