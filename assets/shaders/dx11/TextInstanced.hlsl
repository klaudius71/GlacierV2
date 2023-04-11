#pragma pack_matrix( row_major )

#define MAX_CHARACTERS 128

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

struct GlyphData
{
    float2 TexelPos;
    float2 Size;
};
struct GlyphWorldData
{
    float2 ScreenPos;
    float2 Scale;
};
cbuffer GlyphDataArray : register(b2)
{
    GlyphData Data[MAX_CHARACTERS];
    GlyphWorldData WorldData[MAX_CHARACTERS];
    float4 Color;
    float2 TexSize;
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
    float2 Tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input, uint id : SV_InstanceID)
{
    VS_OUTPUT output;
    output.Tex = (Data[id].TexelPos.xy / TexSize) + input.Tex * float2(Data[id].Size / TexSize);

    float4x4 WorldShifted = World;
    WorldShifted[0] *= WorldData[id].Scale.x;
    WorldShifted[1] *= WorldData[id].Scale.y;
    WorldShifted[3] += mul(float4(WorldData[id].ScreenPos.xy, 0.0, 0.0), World);

    output.Pos = mul(float4(input.Pos, 1.0), WorldShifted);
    output.Pos = mul(output.Pos, Projection);
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(Color.rgb, mainTexture.Sample(aSampler, input.Tex).r);
}