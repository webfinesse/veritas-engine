#define E_LIGHTYPE_DIRECTIONAL 1
#define E_LIGHTYPE_SPOTLIGHT 2
#define E_LIGHTYPE_POINT 3

struct LightOut
{
    float4 Diffuse;
    float4 Specular;
};

struct CalculateLightSettings
{
    float4 VertexPositionW;
    float3 ViewDirection;
    float3 Normal;
    Material Material;
};

// from http://www.3dgep.com/texturing-lighting-directx-11/#Diffuse_1

float CalculateAttenuation(Light light, float d)
{
    return 1.0f / (light.ConstantAttenuation + (light.LinearAttenuation * d) + (light.QuadraticAttenuation * d * d));
}

float CalculateSpotlightCone(Light light, float3 lightDirection)
{
    float minCos = cos(light.SpotAngle);
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot(light.Direction.xyz, -lightDirection);
    return smoothstep(minCos, maxCos, cosAngle);
}

float4 CalculateDiffuse(Light light, CalculateLightSettings settings, float3 lightDirection)
{
    float NDotL = max(0, dot(settings.Normal, lightDirection));
    return light.Color * NDotL;
}

float4 CalculateSpecular(Light light, CalculateLightSettings settings, float3 lightDirection)
{
    float3 reflection = normalize(reflect(-lightDirection, settings.Normal));
    float RDotV = max(0, dot(reflection, settings.ViewDirection));

    float4 specular = pow(RDotV, settings.Material.specular.a);
    
    return light.Color * specular;
}

LightOut CalculatePointLight(Light light, CalculateLightSettings settings)
{
    LightOut result;

    float3 lightDirection = (light.Position - settings.VertexPositionW).xyz;
    float distance = length(lightDirection);
    lightDirection = lightDirection / distance;
 
    float attenuation = CalculateAttenuation(light, distance);
 
    result.Diffuse = CalculateDiffuse(light, settings, lightDirection) * attenuation;
    result.Specular = CalculateSpecular(light, settings, lightDirection) * attenuation;
 
    return result;
}

LightOut CalculateDirectionalLight(Light light, CalculateLightSettings settings)
{
    LightOut result;

    float3 lightDirection = -light.Direction.xyz;

    result.Diffuse = CalculateDiffuse(light, settings, lightDirection);
    result.Specular = CalculateSpecular(light, settings, lightDirection);

    return result;
}

LightOut CalculateSpotlight(Light light, CalculateLightSettings settings)
{
    LightOut result;

    float3 lightDirection = (light.Position - settings.VertexPositionW).xyz;
    float distance = length(lightDirection);
    lightDirection = lightDirection / distance;
 
    float attenuation = CalculateAttenuation(light, distance);
    float spotIntensity = CalculateSpotlightCone(light, lightDirection);

    result.Diffuse = CalculateDiffuse(light, settings, lightDirection) * attenuation * spotIntensity;
    result.Specular = CalculateSpecular(light, settings, lightDirection) * attenuation * spotIntensity;

    return result;
}