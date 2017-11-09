#include "Types.hlsli"

cbuffer cbSkinningPalette : register(b2)
{
	float4x4 skinningPalette[MAX_BONES];
};

VertexOut main(SkinnedVertexIn vIn)
{
	float4x4 boneTransform = mul(skinningPalette[vIn.jointIndicies[0]], vIn.jointWeights[0]);
	float weight;
	for (int i = 1; i < MAX_JOINTS; i++)
	{
		if (vIn.jointIndicies[i] != 255)
		{
			if (i != 3)
			{
				weight = vIn.jointWeights[i];
			}
			else
			{
				weight = 1 - (vIn.jointWeights[0] + vIn.jointWeights[1] + vIn.jointWeights[2]);
			}

			boneTransform += mul(skinningPalette[vIn.jointIndicies[i]], weight);
		}
	}

	VertexOut vOut;

	float4 posL = mul(boneTransform, float4(vIn.positionL, 1.0));

	vOut.posW = mul(posL, worldTransform).xyz;
	vOut.normalW = mul( mul(boneTransform, vIn.normalL), (float3x3) worldInverseTranspose);
	vOut.normalW = normalize(vOut.normalW);
	vOut.tangentW = mul(mul(boneTransform, vIn.tangentL), (float3x3) worldInverseTranspose);
	vOut.tangentW = normalize(vOut.tangentW);
	vOut.binormalW = mul(mul(boneTransform, vIn.binormalL), (float3x3) worldInverseTranspose);
	vOut.binormalW = normalize(vOut.binormalW);
	vOut.diffuseCoord = vIn.diffuseCoord;

	

	matrix wvp = mul(mul(worldTransform, viewMatrix), projMatrix);
	vOut.posH = mul(posL, wvp);

	return vOut;
}