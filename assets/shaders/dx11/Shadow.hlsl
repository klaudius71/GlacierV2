#pragma pack_matrix( row_major )

//  Constant Buffers
cbuffer LightspaceMatrices : register(b0)
{
    float4x4 Lightspace;
}

cbuffer InstanceData : register(b1)
{
    float4x4 World;
};



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
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos = mul(float4(input.Pos, 1.0), World);
    output.Pos = mul(output.Pos, Lightspace);
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(1.0, 1.0, 0.0, 1.0);
}