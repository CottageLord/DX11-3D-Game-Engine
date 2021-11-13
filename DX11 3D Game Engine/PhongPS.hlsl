cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};
// for each object's material
cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

Texture2D tex;
SamplerState splr;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord) : SV_Target
{
    // renormalize interpolated normal
    viewNormal = normalize(viewNormal);
	// fragment to light vector data
    const float3 vToL = lightPos - viewPos;
	const float  distToL = length(vToL);
	const float3 dirToL = vToL / distToL;
	// attenuation
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, viewNormal));
	// reflected light vector
    const float3 w = viewNormal * dot(vToL, viewNormal);
	const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, 
	// narrow with power function and attenuation
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);
	// final color, differenciate specular, in case the black texture cancels out all specular
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}
