struct VSInput
{
	float3 Position : POSITION;
	float3 Color : COLOR;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 WorldPos : TEXCOORD1;
	float3 Normal : TEXCOORD2;
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

SamplerState cubemapSampler : register(s1, space0);

Texture2D cubemap[6] : register(t2, space0);

VSOutput VSMain(VSInput input)
{
	VSOutput output;
	output.WorldPos = input.Position;
	output.Position = mul(uniformData.P, float4(mul((float3x3)uniformData.V, output.WorldPos), 1.0));
	output.Position.y *= -1.0;
	output.Position.xyzw = output.Position.xyww;
	output.uv = input.UV;
	return output;
}

float4 PSMain(VSOutput input) : SV_Target
{
	float factor = saturate(dot(normalize(input.WorldPos), float3(0.0, 1.0, 0.0)));
	//float3 color = uniformData.TopColor.rgb * factor + uniformData.BottomColor.rgb * (1.0 - factor);
	float3 color = float3(0, 0, 0);
	float mag = max(max(abs(input.WorldPos.x), abs(input.WorldPos.y)), abs(input.WorldPos.z));
	if (mag == abs(input.WorldPos.x))
	{
		if (input.WorldPos.x > 0)
		{
			color = cubemap[0].Sample(cubemapSampler, float2(input.WorldPos.z * -0.5 + 0.5, input.WorldPos.y * -0.5 + 0.5)).rgb;
		}
		else
		{
			color = cubemap[1].Sample(cubemapSampler, float2(input.WorldPos.z * 0.5 + 0.5, input.WorldPos.y * -0.5 + 0.5)).rgb;
		}
	}
	else if (mag == abs(input.WorldPos.y))
	{
		if (input.WorldPos.y > 0)
		{
			color = cubemap[2].Sample(cubemapSampler, float2(input.WorldPos.x * 0.5 + 0.5, input.WorldPos.z * 0.5 + 0.5)).rgb;
		}
		else
		{
			color = cubemap[3].Sample(cubemapSampler, float2(input.WorldPos.x * 0.5 + 0.5, input.WorldPos.z * -0.5 + 0.5)).rgb;
		}
	}
	else
	{
		if (input.WorldPos.z > 0)
		{
			color = cubemap[4].Sample(cubemapSampler, float2(input.WorldPos.x * 0.5 + 0.5, input.WorldPos.y * -0.5 + 0.5)).rgb;
		}
		else
		{
			color = cubemap[5].Sample(cubemapSampler, float2(input.WorldPos.x * -0.5 + 0.5, input.WorldPos.y * -0.5 + 0.5)).rgb;
		}
	}
	return float4(color, 1);
}