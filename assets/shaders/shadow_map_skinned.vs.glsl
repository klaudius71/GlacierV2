#version 460 core

#define MAX_BONES 100

layout (location = 0) in vec3 position;
layout (location = 6) in uvec4 joint_ids;
layout (location = 7) in vec4 joint_weights;

layout (std140, binding = 2) uniform LightspaceMatrices
{
    mat4 lightspace_dir_light;
};

uniform mat4 world_matrix;
uniform mat4 bone_matrices[MAX_BONES];

void main()
{
    mat4 skin_matrix = joint_weights.x * bone_matrices[joint_ids.x] +
                        joint_weights.y * bone_matrices[joint_ids.y] +
                        joint_weights.z * bone_matrices[joint_ids.z] +
                        joint_weights.w * bone_matrices[joint_ids.w];

    gl_Position = lightspace_dir_light * world_matrix * skin_matrix * vec4(position, 1.0f);
}