#version 420

#define MAX_BONES 100

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in uint tex_id;
layout (location = 3) in vec3 normal;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 bitangent;
layout (location = 6) in uvec4 joint_ids;
layout (location = 7) in vec4 joint_weights;

layout (std140, binding = 0) uniform Matrices
{
    mat4 proj_matrix;
    mat4 view_matrix;
};

layout (std140, binding = 2) uniform LightspaceMatrices
{
	mat4 lightspace;
};

uniform mat4 world_matrix;
uniform mat4 bone_matrices[MAX_BONES];

out VS_OUT
{
    vec4 position_cameraspace;
    vec4 position_lightspace;
    vec2 tex_coord;
    flat uint tex_id;
    mat3 TBN_cameraspace;
} vs_out;

void main()
{
    mat4 skin_matrix = joint_weights.x * bone_matrices[joint_ids.x] +
                    joint_weights.y * bone_matrices[joint_ids.y] +
                    joint_weights.z * bone_matrices[joint_ids.z] +
                    joint_weights.w * bone_matrices[joint_ids.w];

    const vec4 world_pos = world_matrix * skin_matrix * vec4(position, 1.0f);
    vs_out.position_lightspace = lightspace * world_pos;
    vs_out.position_cameraspace = view_matrix * world_pos;
    gl_Position = proj_matrix * vs_out.position_cameraspace;
    mat4 normal_matrix = transpose(inverse(view_matrix * world_matrix * skin_matrix));
    vs_out.tex_coord = uv;
    vs_out.tex_id = tex_id;

    vec3 T = normalize(vec3(normal_matrix * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(normal_matrix * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(normal_matrix * vec4(normal, 0.0)));
    vs_out.TBN_cameraspace = mat3(T, B, N);
}
