//[numthreads(1, 1, 1)]
cbuffer PointLightCBuf : register(b2)
{
    float3 viewLightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
}; 