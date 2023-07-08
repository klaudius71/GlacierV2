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
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input, uint instanceID : SV_InstanceID)
{
    VS_OUTPUT output;
    output.Tex = (Data[instanceID].TexelPos.xy / TexSize) + input.Tex * float2(Data[instanceID].Size / TexSize);

    float4x4 WorldShifted = World;
    WorldShifted[0] *= WorldData[instanceID].Scale.x;
    WorldShifted[1] *= WorldData[instanceID].Scale.y;
    WorldShifted[3] += mul(float4(WorldData[instanceID].ScreenPos.xy, 0.0, 0.0), World);

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