#include "ShaderOps.hlsl"
#include "LightVectorData.hlsl"

#include "PointLight.hlsl"
// for each object's material
cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

Texture2D tex : register(t0);
Texture2D toon : register(t1);
SamplerState splr;

float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord) : SV_Target
{
    // renormalize interpolated normal
    viewNormal = normalize(viewNormal);
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// diffuse
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);
	// specular
    const float3 specular = Speculate(diffuseColor, diffuseIntensity, viewNormal, lv.vToL, viewFragPos, att, specularPower);
    
    // maps from (-1, 1) to (0, 1)
    const float2 toonTexCoord = float2(dot(viewNormal, normalize(lv.dirToL)) / 2 + 0.5f, 0.0f);
    const float3 toonification = tex.Sample(splr, tc).rgb - (float3(1.0f, 1.0f, 1.0f) - toon.Sample(splr, toonTexCoord).rgb) / 4;
    // final color, differenci ate specular, in case the black texture cancels out all specular
    return float4(saturate((diffuse + ambient) * toonification + specular), 1.0f);
    //return float4(saturate((diffuse + ambient) * (dot(viewNormal, lv.dirToL) > 0 ? tex.Sample(splr, tc).rgb : tex.Sample(splr, tc).rgb - toonification) + specular), 1.0f);
}
 