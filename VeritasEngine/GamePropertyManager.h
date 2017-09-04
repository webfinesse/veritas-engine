#pragma once

#include <memory>
#include <any>

#include "../Includes/AssocVector/assocvector.hpp"
#include "StringHash.h"
#include "IDeserializeMapping.h"
#include "GameObjectProperty.h"

namespace VeritasEngine
{
	class GamePropertyManager
	{
	public:
		GamePropertyManager(std::shared_ptr<IDeserializeMapping> deserializeMapping)
			: m_deserializeMapping{ std::move(deserializeMapping) }, m_properties{}
		{
			
		}

		GamePropertyManager(const GamePropertyManager&& other) noexcept
			: m_deserializeMapping{ std::move(other.m_deserializeMapping)}, m_properties{ std::move(other.m_properties) }
		{
			
		}

		GamePropertyManager& operator=(const GamePropertyManager&& other) noexcept
		{
			if(this != &other)
			{
				m_deserializeMapping = std::move(other.m_deserializeMapping);
				m_properties = std::move(other.m_properties);
			}

			return *this;
		}

		template <typename T>
		auto RegisterProperty(const char* name, StringHash jsonTag)
		{
			m_properties[jsonTag] = std::make_any<GameObjectProperty<T>>(name);

			auto propAddress = GetProperty<T>(jsonTag);

			m_deserializeMapping->Register(jsonTag, [propAddress](Engine& engine, GameObjectHandle handle, JsonValue& values) -> void {
				propAddress->Deserialize(engine, handle, values);
			});

			return propAddress;
		}

		template <typename T>
		auto GetProperty(const char* name)
		{
			return GetProperty<T>(Hash(name));
		}

		template <typename T>
		auto GetProperty(StringHash name)
		{
			auto foo = &m_properties[name];
			return std::any_cast<GameObjectProperty<T>>(foo);
		}

	private:
		std::shared_ptr<IDeserializeMapping> m_deserializeMapping;
		AssocVector<StringHash, std::any> m_properties;
	};
}
