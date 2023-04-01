#pragma pack_matrix( row_major )

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
VS_OUTPUT VS( VS_INPUT input)
{
	VS_OUTPUT output;
	output.Pos = mul(float4(input.Pos, 1.0f), World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	return mainTexture.Sample( aSampler, input.Tex);
}