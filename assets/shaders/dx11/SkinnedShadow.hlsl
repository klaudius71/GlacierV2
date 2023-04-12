#pragma pack_matrix( row_major )

#define NUM_BONES 100

//  Constant Buffers
cbuffer LightspaceMatrices : register(b0)
{
    float4x4 Lightspace;
}

cbuffer InstanceData : register(b1)
{
    float4x4 World;
};

cbuffer JointData : register(b2)
{
    float4x4 JointMatrices[NUM_BONES];
}


struct VS_INPUT
{
    float3 Pos : POSITION;
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
    float4x4 Skin = input.JointWeights.x * JointMatrices[input.JointIDs.x] +
                    input.JointWeights.y * JointMatrices[input.JointIDs.y] +
                    input.JointWeights.z * JointMatrices[input.JointIDs.z] +
                    input.JointWeights.w * JointMatrices[input.JointIDs.w];
    
    VS_OUTPUT output;
    output.Pos = mul(float4(input.Pos, 1.0), Skin);
    output.Pos = mul(output.Pos, World);
    output.Pos = mul(output.Pos, Lightspace);
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(0.0, 0.0, 0.0, 1.0);
}