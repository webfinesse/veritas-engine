#pragma once

#include <memory>

#include "DynamicLibraryHelper.h"
#include "VertexTypeHandle.h"

namespace VeritasEngine
{
	class VertexBuffer;

	class EXPORT IVertexBufferManager
	{
	public:
		virtual ~IVertexBufferManager() = default;

		virtual void RegisterVertexFormat(VertexTypeHandle handle, size_t sizeOfVertex) = 0;
		virtual std::shared_ptr<VeritasEngine::VertexBuffer> GetBuffer(VertexTypeHandle handle) const = 0;
	};
}