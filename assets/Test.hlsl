#define PI 3.1415926535

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
	float3 Color : COLOR;
    float2 uv : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 WorldPos : TEXCOORD2;
};

struct uniformData
{
    float4x4 MVP;
    float4 lightPos;
    float4 cameraPos;
    float4 surfaceData;
};

uniformData data : register(b0, space0);
Texture2D tex : register(t1, space0);

SamplerState testSampler : register(s1, space0);

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

float4 PBRLighting(float3 albedo, float NoL, float NoH, float NoV, float HoV, float roughness, float metallic)
{
    float F0 = lerp(0.04, albedo, metallic);
    float ks = F0 + (1.0 - F0) * pow(saturate(1.0 - HoV), 5);
    float kd = (1.0f - ks) * (1.0f - metallic);

    float3 diffuse = kd * albedo / PI;
    float3 specular = D_GGX(NoH, roughness) * G_Smith(NoV, roughness) * G_Smith(NoL, roughness) / (4.0 * NoV * NoL);
    float3 ambient = float3(0.2f, 0.2f, 0.2f);

    return float4((diffuse + saturate(specular)) * (NoL * float3(1, 1, 1) + ambient), 1.0f);
}

VSOutput vert(VSInput vin)
{
	VSOutput vout = (VSOutput)0;

	vout.Position = mul(data.MVP, float4(vin.Position, 1.0));
	vout.Position.y *= -1.0f;
    vout.WorldPos = vin.Position;
    vout.Normal = normalize(vin.Normal);
	vout.Color = vin.Color;
    vout.uv = vin.UV;

	return vout;
}

float4 frag(VSOutput pin) : SV_TARGET
{
    float3 L = normalize(data.lightPos.xyz - pin.WorldPos);
    float3 V = normalize(data.cameraPos.xyz - pin.WorldPos);
    float3 H = normalize(L + V);
    
    float NoL = saturate(dot(pin.Normal, L));
    float NoH = saturate(dot(pin.Normal, H));
    float NoV = saturate(dot(pin.Normal, V));
    float HoV = saturate(dot(H, V));

    //float3 albedo = tex.Sample(testSampler, pin.uv).rgb * data.surfaceData.rgb;
    float3 albedo = pin.Color;//data.surfaceData.rgb;
    return float4(albedo, 1.0f);
	// return PBRLighting(albedo, NoL, NoH, NoV, HoV, data.surfaceData.w, 0.0f);
}