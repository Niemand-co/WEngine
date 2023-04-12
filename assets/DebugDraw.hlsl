struct VSInput
{
	float3 Position : POSITION;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
};

struct SceneData
{
	float4x4 VP;
};

cbuffer sceneData : register(b0) { SceneData sceneData; }

VSOutput VSMain(VSInput input)
{
	VSOutput output;
	// output.Position = mul(sceneData.VP, float4(input.Position, 1.0));
	output.Position = float4(input.Position, 1.0);
	// output.Position.y *= -1.0f;
    // output.Color = input.Color;
	return output;
}

float4 PSMain(VSOutput input) : SV_Target
{
    return float4(0.0, 1.0, 0.0, 1.0);
}