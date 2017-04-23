#pragma once

namespace VeritasEngine
{
	class BufferIndicies;

	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;

		virtual BufferIndicies AddVerticies(unsigned char* verticies, size_t numVerticies) = 0;
		virtual size_t GetVertexSize() const = 0;
		virtual void* GetNativeBuffer() const = 0;
	};
}