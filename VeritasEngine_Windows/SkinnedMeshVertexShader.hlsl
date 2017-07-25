#include "Types.hlsli"

VertexOut main(SkinnedVertexIn vIn)
{
	VertexOut vOut;

	float4 posL = float4(vIn.positionL, 1.0);

	vOut.posW = mul(posL, worldTransform).xyz;
	vOut.normalW = mul(vIn.normalL, (float3x3)worldInverseTranspose);
	vOut.normalW = normalize(vOut.normalW);
	vOut.tangentW = mul(vIn.tangentL, (float3x3) worldInverseTranspose);
	vOut.tangentW = normalize(vOut.tangentW);
	vOut.binormalW = mul(vIn.binormalL, (float3x3) worldInverseTranspose);
	vOut.binormalW = normalize(vOut.binormalW);
	vOut.diffuseCoord = vIn.diffuseCoord;

	matrix wvp = mul(mul(worldTransform, viewMatrix), projMatrix);
	vOut.posH = mul(posL, wvp);

	return vOut;
}