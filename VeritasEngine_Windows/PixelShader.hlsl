#include "Types.hlsli"

#include "LightingHelper.hlsli"

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularTexture : register(t2);
Texture2D transparentTexture : register(t3);

SamplerState MeshTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

LightOut CalculateLighting(CalculateLightSettings settings)
{
    LightOut totalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

    [unroll]
    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        LightOut result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

        if (!lights[i].Enabled)
        {
            continue;
        }

        switch (lights[i].Type)
        {
            case E_LIGHTYPE_DIRECTIONAL:
                result = CalculateDirectionalLight(lights[i], settings);
                break;
            case E_LIGHTYPE_POINT:
                result = CalculatePointLight(lights[i], settings);
                break;
            case E_LIGHTYPE_SPOTLIGHT:
                result = CalculateSpotlight(lights[i], settings);
                break;
        }

        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    totalResult.Diffuse = saturate(totalResult.Diffuse);
    totalResult.Specular = saturate(totalResult.Specular);

    return totalResult;
}

float4 main(VertexOut pIn) : SV_TARGET
{
    float3 normal = pIn.normalW;

	if ((shaderFlags & ShaderFlags_HasNormalMap) == ShaderFlags_HasNormalMap)
    {
        normal = normalTexture.Sample(MeshTextureSampler, pIn.diffuseCoord).xyz;
        normal = (normal * 2.0f) - 1.0f;
        normal = (normal.x * pIn.tangentW) + (normal.y * pIn.binormalW) + (normal.z * pIn.normalW);
        normal = normalize(normal);
    }

    CalculateLightSettings settings;
    settings.VertexPositionW = float4(pIn.posW, 1);
    settings.ViewDirection = normalize(eyePosition - pIn.posW);
    settings.Normal = normal;
    settings.Material = material;

    LightOut result = CalculateLighting(settings);

    float4 emissive = material.emissive;
    float4 ambient = material.ambient;
    float4 diffuse = material.diffuse * result.Diffuse;
    float4 specular = material.specular * result.Specular;

    float4 texColor = { 1, 1, 1, 1 };

    if((shaderFlags & ShaderFlags_HasDiffuseMap) == ShaderFlags_HasDiffuseMap)
    {
        texColor = diffuseTexture.Sample(MeshTextureSampler, pIn.diffuseCoord);
    }

    float4 specularIntensity = { 1, 1, 1, 1 };

	if ((shaderFlags & ShaderFlags_HasSpecularMap) == ShaderFlags_HasSpecularMap)
    {
        specularIntensity = specularTexture.Sample(MeshTextureSampler, pIn.diffuseCoord);
    }

    float4 color = (emissive + ambient + diffuse + (specular * specularIntensity)) * texColor;

	return color;
}