cbuffer TransformCBuf : register(b0)
{
    matrix viewProj; // the skybox doesn't move, hence don't need model matrix
};

struct VSOut
{
    float3 worldPos : Position; // used in sampling cube texture
    float4 pos : SV_Position; // in screen space, we need to rotate the skybox geometry during sampling
};

VSOut main(float3 pos : Position)
{
    VSOut vso;
    vso.worldPos = pos;
    // set the z 0 to nullify translation
    vso.pos = mul(float4(pos, 0.0f), viewProj);
    // make sure that the depth after w divide will be 1.0 (so that the z-buffering will work)
    // uniform depth of 1.0 is useful in out skybox system (see notes)
    vso.pos.z = vso.pos.w;
    return vso;
}