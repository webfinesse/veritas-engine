#pragma once

#include <vector>

#include "PassBuffer.h"
#include "BufferIndicies.h"

namespace VeritasEngine
{
	struct MaterialInstance;

	struct PerObjectBufferDescription
	{
	public:
		PerObjectBufferDescription()
			: WorldTransform{}, WorldInverseTranspose{}, Material{}, IndexBuffer {}, IndexIndicies{ 0, 0 }, VertexBuffer{}, VertexIndicies{ 0, 0 }, VertexSize{}
		{
		}

		PerObjectBufferDescription(const Matrix4x4& worldTransform, const Matrix4x4& inverseTranspose, const MaterialInstance* material, 
								   void* indexBuffer, const BufferIndicies& indexIndicies,  void* vertexBuffer, const BufferIndicies& vertexIndicies, size_t vertexSize)
			: WorldTransform(worldTransform), WorldInverseTranspose(inverseTranspose), Material(material), IndexBuffer{ indexBuffer }, IndexIndicies{ indexIndicies }, VertexBuffer{ vertexBuffer }, VertexIndicies{ vertexIndicies }, VertexSize{ vertexSize }
		{

		}

		const Matrix4x4 WorldTransform;
		const Matrix4x4 WorldInverseTranspose;
		const MaterialInstance* Material;
		void* IndexBuffer;
		const BufferIndicies& IndexIndicies;
		void* VertexBuffer;
		const BufferIndicies& VertexIndicies;
		const size_t VertexSize;

	};

	class FrameDescription
	{
	public:
		float AspectRatio;
		PassBuffer PassBuffer;
		std::vector<PerObjectBufferDescription> StaticObjects;
		std::vector<PerObjectBufferDescription> AnimatedObjects;
	};
}
