struct VSInput
{
	float3 Position : POSITION;
	float3 Color : COLOR;
	float3 Normal : NORMAL;
};

struct VSOutput
{
	float4 Position : POSITION;
	float3 WorldPos : TEXCOORD0;
	float3 Normal : TEXCOORD1;
};

struct UniformData
{
	float4x4 V;
	float4x4 P;
	float4 CameraPos;
};

UniformData uniformData : register(b0, space0);

VSOutput vert(VSInput input)
{
	VSOutput output;
	output.WorldPos = input.Position;
	output.Position = mul(uniformData.P, float4(mul((float3x3)uniformData.V, output.WorldPos), 1.0));
	return output;
}

float4 frag(VSOutput input) : SV_Target
{
	return float4(1, 1, 1, 1);
}