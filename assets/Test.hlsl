#define PI 3.1415926535

struct VSInput
{
	float3 Position : POSITION;
	float3 Color : COLOR;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float3 Color : COLOR;
    float3 WorldPos : TEXCOORD0;
};

struct SurfaceData
{
    float3 albedo;
    float roughness;
    float metallic;
};
uniform float4x4 MVP : register(b0);
uniform float3 lightPos : register(b1);
uniform SurfaceData surfaceData : register(b2);


float D_GGX(float NoH, float Roughness)
{
    float squareNoH = NoH * NoH;
    float alpha = Roughness * Roughness;
    float squareAlpha = alpha * alpha;

    return (squareAlpha / (PI * pow(squareNoH * (squareAlpha - 1.0f) + 1.0, 2)));

}

float G_Smith(float cos, float Roughness)
{
    float alpha = Roughness * Roughness;
    float squareAlpha = (alpha + 1.0) * (alpha + 1.0) / 8.0;
    return cos / (cos * (1.0 - squareAlpha) + squareAlpha);
}

float4 PBRLighting(float3 albedo, float NoL, float NoH, float NoV, float HoV, float roughness, float metallic, float3 normal, float isGI)
{
    float F0 = lerp(0.04, albedo, metallic);
    float ks = F0 + (1.0 - F0) * pow(saturate(1.0 - HoV), 5);
    float kd = (1.0f - ks) * (1.0f - metallic);

    float3 diffuse = kd * albedo / PI;
    float3 specular = D_GGX(NoH, roughness) * G_Smith(NoV, roughness) * G_Smith(NoL, roughness) / (4.0 * NoV * NoL);

    return float4((diffuse + saturate(specular)) * NoL * float3(1, 1, 1), 1.0f);
}

VSOutput vert(VSInput vin)
{
	VSOutput vout = (VSOutput)0;

	vout.Position = mul(MVP, float4(vin.Position, 1.0));
	vout.Position.y *= -1.0f;
    vout.WorldPos = vin.Position;
	vout.Color = vin.Color;

	return vout;
}

float4 frag(VSOutput pin) : SV_TARGET
{
    
	return float4(0.0f, lightPos.yz, 1.0f);
}