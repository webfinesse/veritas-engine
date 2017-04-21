#pragma once

#include <functional>

#include "../VeritasEngineBase/GameObjectHandle.h"
#include "JsonValue.h"
#include "StringHash.h"
#include "DynamicLibraryHelper.h"

namespace VeritasEngine
{
	class Engine;
	using DESERIALIZEFUNCTION = std::function<void(Engine& engine, GameObjectHandle handle, JsonValue& values)>;

	class EXPORT IDeserializeMapping
	{
	public:
		virtual ~IDeserializeMapping() = default;
		virtual void Register(StringHash jsonTag, const DESERIALIZEFUNCTION function) = 0;
		virtual DESERIALIZEFUNCTION* const GetDeserializer(StringHash jsonTag) = 0;
	};
}