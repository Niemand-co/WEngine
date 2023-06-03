struct VSIn
{
    int ID : SV_VertexID;
};

struct VSOut
{
    float4 Position : SV_POSITION;
    float4 texcoord : TEXCOORD0;
    float3 frag_pos : TEXCOORD1;
};


struct UniformData
{
    float4x4 MVP;
    float4 position[36];
    float4 attr[36];
    half3 Color;
    half3 padding1;
    half Roughness;
    half padding2;
};

UniformData ubuf : register(b0, space0);

Texture2D tex : register(t1, space0);

SamplerState DummySampler;

VSOut VSMain(VSIn VIn)
{
    VSOut o;
    o.texcoord = ubuf.attr[VIn.ID];
    o.Position = mul(ubuf.MVP, ubuf.position[VIn.ID]);
    o.frag_pos = o.Position.xyz;
    return o;
}

void PSMain(VSOut PIn,
            out float4 FragColor : SV_Target0)
{
    float3 dX = ddx(PIn.frag_pos);
    float3 dY = ddy(PIn.frag_pos);
    float3 normal = normalize(cross(dX, dY));
    float light = max(0.0, dot(float3(0.4239999949932098388671875, 0.5659999847412109375, 0.7070000171661376953125), normal));
    FragColor = float4(ubuf.Color.xyz, 1.0f);//tex.Sample(DummySampler, PIn.texcoord.xy) * light;
}