#pragma pack_matrix( row_major )

#define NUM_BONES 100

Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

cbuffer CamData : register(b0)
{
    float4x4 Projection;
    float4x4 View;
}

cbuffer InstanceData : register(b1)
{
    float4x4 World;
};

cbuffer LightspaceData : register(b4)
{
    float4x4 Lightspace;
}

cbuffer JointData : register(b5)
{
    float4x4 JointMatrices[NUM_BONES];
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
    float4 Pos_Lightspace : POSITION1;
    float2 Tex : TEXCOORD;
    float3x3 TBN_CameraSpace : NORMAL;
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
    
    float4x4 NormalMatrix = mul(Skin, World);
    output.Pos_Lightspace = mul(float4(input.Pos, 1.0), NormalMatrix);
    output.Pos_Lightspace = mul(output.Pos_Lightspace, Lightspace);
    NormalMatrix = mul(NormalMatrix, View);
    
    output.Pos = mul(float4(input.Pos, 1.0f), NormalMatrix);
    
    output.Pos_CameraSpace = output.Pos;
    output.Pos = mul(output.Pos, Projection);
    
    output.Tex = input.Tex;
    
    output.TBN_CameraSpace[0] = normalize(mul(float4(input.Tangent, 0.0), NormalMatrix));
    output.TBN_CameraSpace[1] = normalize(mul(float4(input.Bitangent, 0.0), NormalMatrix));
    output.TBN_CameraSpace[2] = normalize(mul(float4(input.Normal, 0.0), NormalMatrix));
    return output;
}