TextureCube smap : register(t3);
SamplerComparisonState ssam : register(s1);

float Shadow(const in float4 shadowPos)
{
    return smap.SampleCmpLevelZero(ssam, normalize(shadowPos.xyz), length(shadowPos.xyz) / 100.0f);
}

/*
//Texture2D smap : register(t3);
TextureCube smap : register(t3);
SamplerComparisonState ssam : register(s1);

#define PCF_RANGE 1

float ShadowLoop_(const in float3 spos)
{
    // PCF 3x3
    float shadowLevel = 0.0f;
    [unroll]
    for (int x = -PCF_RANGE; x <= PCF_RANGE; x++)
    {
        [unroll]
        for (int y = -PCF_RANGE; y <= PCF_RANGE; y++)
        {
            // compares between fetched texel spos.xy and offset texel spos.xy + (x,y), returns 0 or 1
            // the SamplerComparisonState can also be set to fetch multiple texels for comparison
            // the result will be blended and the return value will be floating number between 0 ~ 1
            shadowLevel += smap.SampleCmpLevelZero(ssam, normalize(spos.xyz), length(spos.xyz) / 100.0f);
        }
    }
    return shadowLevel / ((PCF_RANGE * 2 + 1) * (PCF_RANGE * 2 + 1));
}

float Shadow(const in float4 shadowHomoPos)
{
    float shadowLevel = 0.0f;
    const float3 spos = shadowHomoPos.xyz / shadowHomoPos.w;

    if (spos.z > 1.0f || spos.z < 0.0f)
    {
        shadowLevel = 1.0f;
    }
    else
    {
        shadowLevel = ShadowLoop_(spos);
    }
    return shadowLevel;
}
*/