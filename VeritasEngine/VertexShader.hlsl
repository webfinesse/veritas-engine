#include "Types.hlsli"

VertexOut main(VertexIn vIn)
{
	VertexOut vOut;
	
	float4 posL = float4(vIn.positionL, 1.0);

	vOut.posW = mul(posL, worldTransform).xyz;
	vOut.normalW = mul(vIn.normalL, (float3x3)worldInverseTranspose);

	matrix wvp = mul(mul(worldTransform, viewMatrix), projMatrix);
	vOut.posH = mul(posL, wvp);

	return vOut;
}