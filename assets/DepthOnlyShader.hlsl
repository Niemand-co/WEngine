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

struct ObjectData
{
	float4x4 M;
};

SceneData sceneData : register(b0, space0);

ObjectData objectData : register(b1, space0);

VSOutput vert(VSInput input)
{
	VSOutput output;
	output.Position = mul(sceneData.VP, mul(objectData.M, float4(input.Position, 1.0)));
	output.Position.y *= -1.0f;
	return output;
}

void frag(VSOutput input)
{
	
}