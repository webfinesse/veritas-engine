#pragma once

namespace VeritasEngine
{
	class BufferIndicies;

	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;

		virtual BufferIndicies AddIndicies(unsigned int* indicies, size_t numOfIndicies) = 0;
		virtual void* GetNativeBuffer() const = 0;
	};
}