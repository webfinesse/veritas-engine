#pragma once

#include <memory>

#include "DynamicLibraryHelper.h"
#include "VertexTypeHandle.h"

namespace VeritasEngine
{
	class IVertexBuffer;

	class EXPORT IVertexBufferManager
	{
	public:
		virtual ~IVertexBufferManager() = default;

		virtual void RegisterVertexFormat(VertexTypeHandle handle, size_t sizeOfVertex) = 0;
		virtual VeritasEngine::IVertexBuffer* GetBuffer(VertexTypeHandle handle) const = 0;
	};
}