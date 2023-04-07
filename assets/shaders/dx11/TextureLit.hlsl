#pragma pack_matrix( row_major )

struct PhongADS
{
    float3 ambient; float pad0;
    float3 diffuse; float pad1;
    float3 specular;
    float shininess;
};
struct DirLight
{
    PhongADS LightProperties;
    float3 Direction; // world direction
};

Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

Texture2D normalTexture : register(t1);
SamplerState nSampler : register(s1);

cbuffer CamData : register(b0)
{
    float4x4 Projection;
    float4x4 View;
}

cbuffer InstanceData : register(b1)
{
    float4x4 World;
};

cbuffer MaterialData : register(b2)
{
    PhongADS Material;
}

cbuffer DirectionalLightData : register(b3)
{
    DirLight DirectionalLight;
}

//--------------------------------------------------------------------------------------

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
    uint Id : TEXID;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : TANGENT1;
    uint4 JointIDs : BLENDINDICES;
    float4 JointWeights : BLENDWEIGHT;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Pos_CameraSpace : POSITION;
    float2 Tex : TEXCOORD;
    float3x3 TBN_CameraSpace : NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos = mul(float4(input.Pos, 1.0f), World);
    output.Pos = mul(output.Pos, View);
    
    output.Pos_CameraSpace = output.Pos;
    output.Pos = mul(output.Pos, Projection);
    
    output.Tex = input.Tex;
    
    float4x4 normal_matrix = mul(World, View);
    
    output.TBN_CameraSpace[0] = normalize(mul(float4(input.Tangent, 0.0), normal_matrix));
    output.TBN_CameraSpace[1] = normalize(mul(float4(input.Bitangent, 0.0), normal_matrix));
    output.TBN_CameraSpace[2] = normalize(mul(float4(input.Normal, 0.0), normal_matrix));
    
    return output;
}

float3 PhongModel(PhongADS mat, PhongADS light, float3 L, float3 normal, float3 eyeDir)
{
    float3 diffuse, spec;

    float diffuseFactor = dot(-L, normal);

    if (diffuseFactor > 0.0f)
    {
        diffuse = diffuseFactor * mat.diffuse * light.diffuse;

        float3 r = reflect(L, normal);
        float spec_val = max(dot(r, eyeDir), 0.0);
        float specFactor = pow(spec_val, mat.shininess);
        spec = specFactor * mat.specular * light.specular;
    }
    else
    {
        diffuse = float3(0, 0, 0);
        spec = float3(0, 0, 0);
    }
    
    return diffuse + spec;
}
float3 CalcDirectionalLight(PhongADS mat, DirLight light, float3 normal, float3 eyeDir)
{
    const float3 ambient = mat.ambient * light.LightProperties.ambient;
    const float3 lightDir_cameraspace = mul(float4(light.Direction, 0.0f), View).xyz;
    return ambient + PhongModel(mat, light.LightProperties, lightDir_cameraspace, normal, eyeDir);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{   
    float4 Normal_TangentSpace = (normalTexture.Sample(nSampler, input.Tex) * 2.0) - 1.0;
    float3 Normal_CameraSpace = normalize(mul(Normal_TangentSpace.xyz, input.TBN_CameraSpace));
    
    const float3 eyeDir = normalize(-input.Pos_CameraSpace).xyz;
    
    float3 outColor = float3(0.0, 0.0, 0.0);
    outColor += CalcDirectionalLight(Material, DirectionalLight, Normal_CameraSpace, eyeDir);
    
    return mainTexture.Sample(aSampler, input.Tex) * float4(outColor, 1.0f);
}