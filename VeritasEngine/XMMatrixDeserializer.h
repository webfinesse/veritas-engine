#ifndef H_XMMATRIXDESERIALIZER
#define H_XMMATRIXDESERIALIZER

#include <DirectXPackedVector.h>

class XMMatrixDeserializer
{
	DirectX::XMMATRIX Deserialize()
	{
		DirectX::XMFLOAT4X4 result;
		result._11;

		return XMLoadFloat4x4(&result);
	}
};

#endif