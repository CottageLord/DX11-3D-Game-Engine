cbuffer CBuf
{
	matrix modelView;
	matrix modelViewProj;
};

struct VSOut
{
	float3 camPos : Position;
	float3 normal : Normal;
	float3 color : Color;
	float4 screenPos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float3 color : Color)
{
	VSOut vso;
	vso.camPos = (float3) mul(float4(pos, 1.0f), modelView);
	vso.normal = mul(n, (float3x3) modelView);
	vso.screenPos = mul(float4(pos, 1.0f), modelViewProj);
	vso.color = color;
	return vso;
}