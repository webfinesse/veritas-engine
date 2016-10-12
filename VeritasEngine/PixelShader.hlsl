#include "Types.hlsli"

LightOut ComputeDirectionalLight(Material m, DirectionalLight l, float3 normal, float3 toEye)
{
	LightOut result;

	result.ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	result.diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	result.specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -l.direction;
		result.ambient = m.ambient * l.ambient;

	float diffuseFactor = dot(lightVec, normal);

	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), m.specular.w);
		result.diffuse = diffuseFactor * m.diffuse * l.diffuse;
		result.specular = specFactor * m.specular * l.specular;
	}

	return result;
}

float4 main(VertexOut pIn) : SV_TARGET
{
	pIn.normalW = normalize(pIn.normalW);

	float3 toEye = normalize(eyePosition - pIn.posW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	LightOut light = ComputeDirectionalLight(material, directionalLight, pIn.normalW, toEye);
	ambient += light.ambient;
	diffuse += light.diffuse;
	spec += light.specular;

	float4 color = ambient + diffuse + spec;
	color.a = material.diffuse.a;

	return color;
}