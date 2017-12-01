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

		if (weight > 0.0001) // there can be some round error in the weight that puts it close to 0
		{
			boneTransform += skinningPalette[vIn.jointIndicies[i]] * weight;
		}		
	}

	VertexOut vOut;

	float4 posL = float4(vIn.positionL, 1.0);
	posL = mul(boneTransform, posL);

	vOut.posW = (worldTransform, posL).xyz;
	vOut.normalW = mul((float3x3)worldInverseTranspose, mul(boneTransform, vIn.normalL));
	vOut.normalW = normalize(vOut.normalW);
	vOut.tangentW = mul((float3x3) worldInverseTranspose, mul(boneTransform, vIn.tangentL));
	vOut.tangentW = normalize(vOut.tangentW);
	vOut.binormalW = mul((float3x3) worldInverseTranspose, mul(boneTransform, vIn.binormalL));
	vOut.binormalW = normalize(vOut.binormalW);
	vOut.diffuseCoord = vIn.diffuseCoord;

	matrix wvp = mul(mul(projMatrix, viewMatrix), worldTransform);
	vOut.posH = mul(wvp, posL);

	return vOut;
}