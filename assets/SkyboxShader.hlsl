struct VSInput
{
	float3 Position : POSITION;
	float3 Color : COLOR;
	float3 Normal : NORMAL;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float3 WorldPos : TEXCOORD0;
	float3 Normal : TEXCOORD1;
};

struct UniformData
{
	float4x4 V;
	float4x4 P;
	float4 CameraPos;
	float4 TopColor;
	float4 BottomColor;
};

UniformData uniformData : register(b0, space0);

VSOutput vert(VSInput input)
{
	VSOutput output;
	output.WorldPos = input.Position;
	output.Position = mul(uniformData.P, float4(mul((float3x3)uniformData.V, output.WorldPos), 1.0));
	output.Position.y *= -1.0;
	output.Position.xyzw = output.Position.xyww;
	return output;
}

float4 frag(VSOutput input) : SV_Target
{
	float factor = saturate(dot(normalize(input.WorldPos), float3(0.0, 1.0, 0.0)));
	float3 color = uniformData.TopColor.rgb * factor + uniformData.BottomColor.rgb * (1.0 - factor);
	return float4(color, 1);
}