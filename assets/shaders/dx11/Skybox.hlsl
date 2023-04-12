#pragma pack_matrix( row_major )

TextureCube mainTexture : register(t0);

SamplerState aSampler : register(s0);

cbuffer CamData : register(b0)
{
    float4x4 Projection;
    float4x4 View;
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
    float3 Tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    float4x4 view = View;
    view[3] = float4(0.0, 0.0, 0.0, 1.0);
    float4 proj_view_pos = mul(float4(input.Pos, 1.0), view);
    proj_view_pos = mul(proj_view_pos, Projection);
    
    VS_OUTPUT output;
    output.Pos = proj_view_pos.xyww;
    output.Tex = input.Pos;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    return mainTexture.Sample(aSampler, input.Tex);
}