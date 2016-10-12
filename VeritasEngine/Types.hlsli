struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
};

struct VertexIn
{
	float3 positionL: POSITION;
	float4 normalL: NORMAL;
};

struct VertexOut
{
	float4 posH: SV_POSITION;
	float3 posW: POSITION;
	float3 normalW: NORMAL;
};

struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float pad;
};

struct LightOut
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
};

cbuffer cbCamera: register(b0)
{
	float4x4 viewMatrix;
	float4x4 projMatrix;
	float3 eyePosition;
	float pad;
};

cbuffer cbPerObject : register(b1)
{
	float4x4 worldTransform;
	float4x4 worldInverseTranspose;
	Material material;
};

cbuffer cbPerFrame : register(b2)
{
	DirectionalLight directionalLight;
};