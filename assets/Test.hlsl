struct VSInput
{
	float3 Position : POSITION;
	float3 Color : COLOR;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float3 Color : COLOR;
};

uniform float4x4 MVP : register(b0);

VSOutput vert(VSInput vin)
{
	VSOutput vout = (VSOutput)0;

	vout.Position = mul(MVP, float4(vin.Position, 1.0));
	vout.Position.y *= -1.0f;
	vout.Color = vin.Color;

	return vout;
}

float4 frag(VSOutput pin) : SV_TARGET
{
	return float4(pin.Color, 1.0);
}