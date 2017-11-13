#include "Types.hlsli"

VertexOut main(VertexIn vIn)
{
	VertexOut vOut;
	
	float4 posL = float4(vIn.positionL, 1.0);

	vOut.posW = mul(worldTransform, posL).xyz;
	vOut.normalW = mul((float3x3)worldInverseTranspose, vIn.normalL);
    vOut.normalW = normalize(vOut.normalW);
	vOut.tangentW = mul((float3x3) worldInverseTranspose, vIn.tangentL);
	vOut.tangentW = normalize(vOut.tangentW);
	vOut.binormalW = mul((float3x3) worldInverseTranspose, vIn.binormalL);
	vOut.binormalW = normalize(vOut.binormalW);
	vOut.diffuseCoord = vIn.diffuseCoord;

	matrix wvp = mul(mul(projMatrix, viewMatrix), worldTransform);
	vOut.posH = mul(wvp, posL);

	return vOut;
}