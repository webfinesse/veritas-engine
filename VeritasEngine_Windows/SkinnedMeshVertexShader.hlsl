#include "Types.hlsli"

cbuffer cbSkinningPalette : register(b2)
{
	float4x4 skinningPalette[MAX_BONES];
};

VertexOut main(SkinnedVertexIn vIn)
{
	float4x4 boneTransform = 0;

	float weight;
	for (int i = 0; i < MAX_JOINTS; i++)
	{
		if (i != 3)
		{
			weight = vIn.jointWeights[i];
		}
		else
		{
			weight = 1 - (vIn.jointWeights[0] + vIn.jointWeights[1] + vIn.jointWeights[2]);
		}

		if (weight > 0) 
		{
			boneTransform += skinningPalette[vIn.jointIndicies[i]] * weight;
		}		
	}

	VertexOut vOut;

	float4 posL = float4(vIn.positionL, 1.0);
	matrix finalWorldTransform = mul(boneTransform, worldTransform);
	posL = mul(finalWorldTransform, posL);

	vOut.posW = posL.xyz;
	vOut.normalW = mul((float3x3)worldInverseTranspose, vIn.normalL);
	vOut.normalW = normalize(vOut.normalW);
	vOut.tangentW = mul((float3x3) worldInverseTranspose, vIn.tangentL);
	vOut.tangentW = normalize(vOut.tangentW);
	vOut.binormalW = mul((float3x3) worldInverseTranspose, vIn.binormalL);
	vOut.binormalW = normalize(vOut.binormalW);
	vOut.diffuseCoord = vIn.diffuseCoord;

	matrix wvp = mul(projMatrix, viewMatrix);
	//matrix wvp = mul(mul(projMatrix, viewMatrix), finalWorldTransform);
	vOut.posH = mul(wvp, posL);

	return vOut;

	/*VertexOut vOut;
	
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

	return vOut;*/
}