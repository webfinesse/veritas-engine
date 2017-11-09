#define MAX_LIGHTS 8
#define MAX_BONES 64
#define MAX_JOINTS 4
#define MAX_JOINT_WEIGHTS 3

struct SkinnedVertexIn
{
	float3 positionL: POSITION;
	float3 normalL: NORMAL;
	float3 tangentL: TANGENT;
	float3 binormalL : BINORMAL;
	float2 diffuseCoord: TEXCOORD0;
	int4 jointIndicies: JOINTINDICIES;
	float3 jointWeights: JOINTWEIGHTS;
};

struct VertexIn
{
	float3 positionL: POSITION;
	float3 normalL: NORMAL;
	float3 tangentL: TANGENT;
	float3 binormalL : BINORMAL;
	float2 diffuseCoord: TEXCOORD0;
};

struct VertexOut
{
	float4 posH: SV_POSITION;
	float3 posW: POSITION;
	float3 normalW: NORMAL;
	float3 tangentW: TANGENT;
	float3 binormalW : BINORMAL;
	float2 diffuseCoord: TEXCOORD0;
};

struct Light
{
    float4 Position;
    float4 Direction;
    float4 Color;
    float SpotAngle;
    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;
    int Type;
    bool Enabled;
    int2 pad;
};

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
	float specularPower;
	float3 pad;
};

cbuffer cbCamera: register(b0)
{
	float4x4 viewMatrix;
	float4x4 projMatrix;
	float4 eyePosition;
	Light lights[MAX_LIGHTS];
};

cbuffer cbPerObject : register(b1)
{
	float4x4 worldTransform;
	float4x4 worldInverseTranspose;
	Material material;
	bool hasDiffuseMap;
	bool hasNormalMap;
	bool hasSpecularMap;
	float perObjectPad;
};