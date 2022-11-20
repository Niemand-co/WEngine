struct VSInput
{
	float3 Position : POSITION;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
};

struct UniformData
{
	float4x4 M;
	float4x4 VP;
};

UniformData uniformData : register(b0, space0);

VSOutput vert(VSInput input)
{
	VSOutput output;
	output.Position = mul(uniformData.VP, mul(uniformData.M, float4(input.Position, 1.0)));
	output.Position.y *= -1.0f;
	return output;
}

void frag(VSOutput input)
{

}