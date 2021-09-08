cbuffer CBuf
{
	matrix modelView;
	matrix modelViewProj;
};

struct VSOut
{
	float3 camPos : Position;
	float3 normal	: Normal;
	float4 pos		: SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal)
{
	VSOut vso;
	vso.camPos = (float3) mul(float4(pos, 1.0f), modelView);
	// drop translation info, we only want rotation
	vso.normal = mul(n, (float3x3) modelView);
	vso.pos = mul(float4(pos, 1.0f), modelViewProj);
	return vso;
}