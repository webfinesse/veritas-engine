#pragma once

#include <vector>

#include "PassBuffer.h"
#include "BufferIndicies.h"
#include "Skeleton.h"

namespace VeritasEngine
{
	struct MaterialInstance;

	struct PerObjectBufferDescription
	{
	public:
		PerObjectBufferDescription()
		{
		}

		PerObjectBufferDescription(const Matrix4x4& worldTransform, const Matrix4x4& inverseTranspose, const MaterialInstance* material, 
								   void* indexBuffer, const BufferIndicies& indexIndicies,  void* vertexBuffer, const BufferIndicies& vertexIndicies, size_t vertexSize)
			: WorldTransform(worldTransform), WorldInverseTranspose(inverseTranspose), Material(material), IndexBuffer{ indexBuffer }, IndexIndicies{ indexIndicies }, VertexBuffer{ vertexBuffer }, VertexIndicies{ vertexIndicies }, VertexSize{ vertexSize }
		{

		}

		const Matrix4x4 WorldTransform{};
		const Matrix4x4 WorldInverseTranspose{};
		const MaterialInstance* Material{nullptr};
		void* IndexBuffer{nullptr};
		const BufferIndicies& IndexIndicies{0, 0};
		void* VertexBuffer{nullptr};
		const BufferIndicies& VertexIndicies{0, 0};
		const size_t VertexSize{};
	};

	struct PerAnimatedObjectBufferDescription : PerObjectBufferDescription
	{
		PerAnimatedObjectBufferDescription()
			: PerObjectBufferDescription()
		{
			
		}

		PerAnimatedObjectBufferDescription(const Matrix4x4& worldTransform, const Matrix4x4& inverseTranspose, const MaterialInstance* material,
			void* indexBuffer, const BufferIndicies& indexIndicies, void* vertexBuffer, const BufferIndicies& vertexIndicies, size_t vertexSize, const Matrix4x4* skinningPalette)
			: PerObjectBufferDescription(worldTransform, inverseTranspose, material, indexBuffer, indexIndicies, vertexBuffer, vertexIndicies, vertexSize)
		{
			std::memcpy(SkinningPalette, skinningPalette, sizeof(SkinningPalette));
		}

		Matrix4x4 SkinningPalette[MAX_JOINTS];
	};

	class FrameDescription
	{
	public:
		float AspectRatio;
		PassBuffer PassBuffer;
		std::vector<PerObjectBufferDescription> StaticObjects;
		std::vector<PerAnimatedObjectBufferDescription> AnimatedObjects;
	};
}
