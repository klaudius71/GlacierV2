#version 420 core

struct PhongADS
{
	vec3 ambient;
	vec3 diffuse;
	vec4 specular;
};

struct DirectionalLight
{
    PhongADS light_properties;
    vec3 direction; // world direction
};


layout (std140, binding = 0) uniform Matrices
{
    mat4 proj_matrix;
    mat4 view_matrix;
};

layout (std140, binding = 1) uniform DirLight
{
	DirectionalLight dir_light;
};

uniform vec4 color;
uniform PhongADS material;
uniform sampler2D textures[4];

vec3 PhongModel(PhongADS mat, PhongADS light, vec3 L, vec3 normal, vec3 eyeDir)
{
    vec3 diffuse, spec;

	float diffuseFactor = dot(-L, normal);

	if (diffuseFactor > 0.0f)
	{
		diffuse = diffuseFactor * mat.diffuse * light.diffuse;

		vec3 r = reflect(L, normal);
		float spec_val = max(dot(r, eyeDir), 0.0);
		float specFactor = pow(spec_val, mat.specular.w);
		spec = specFactor * mat.specular.xyz * light.specular.xyz;
	}
	else
	{
		diffuse = vec3(0, 0, 0);
		spec = vec3(0, 0, 0);
	}

    return diffuse + spec;
}

vec3 CalcDirectionalLight(PhongADS mat, DirectionalLight light, vec3 normal, vec3 eyeDir/*, float shadow*/)
{
	vec3 ambient = mat.ambient * light.light_properties.ambient;
	const vec3 lightDir_cameraspace = mat3(view_matrix) * light.direction;
    return ambient + PhongModel(mat, light.light_properties, lightDir_cameraspace, normal, eyeDir);
}

in VS_OUT
{
    vec4 position_cameraspace;
    vec3 normal_cameraspace;
    vec2 tex_coord;
    flat uint tex_id;
} fs_in;

out vec4 FragColor;

void main()
{
	vec3 eyeDir = normalize(-fs_in.position_cameraspace.xyz);

	vec3 outColor = CalcDirectionalLight(material, dir_light, fs_in.normal_cameraspace, eyeDir);
    FragColor = texture(textures[fs_in.tex_id], fs_in.tex_coord) * vec4(outColor, 1.0f) * color;
}
