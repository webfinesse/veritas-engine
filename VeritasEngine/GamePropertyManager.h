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
		GamePropertyManager(shared_ptr<IDeserializeMapping> deserializeMapping)
			: m_deserializeMapping{ std::move(deserializeMapping) }
		{
			
		}

		GamePropertyManager(GamePropertyManager&& other) noexcept
			: m_properties{ std::move(other.m_properties) }
		{
			
		}

		GamePropertyManager& operator=(GamePropertyManager&& other) noexcept
		{
			if(this != &other)
			{
				m_properties = std::move(other.m_properties);
			}

			return *this;
		}

		template <typename T>
		void RegisterProperty(const char* name, StringHash jsonTag)
		{
			auto prop = GameObjectProperty<T>(name, jsonTag);
			m_properties[jsonTag].emplace<std::any>(std::move(prop));

			auto propAddress = GetProperty<T>(jsonTag);

			m_deserializeMapping->Register(jsonTag, [propAddress](Engine& engine, GameObjectHandle handle, JsonValue& values) -> void {
				propAddress->Deserialize(engine, handle, values);
			});
		}

		template <typename T>
		GameObjectProperty<T>* GetProperty(const char* name)
		{
			return GetProperty<T>(Hash(name));
		}

		template <typename T>
		GameObjectProperty<T>* GetProperty(StringHash name)
		{
			return any_cast<GameObjectProperty<T>*>(m_properties[name]);
		}

	private:
		shared_ptr<IDeserializeMapping> m_deserializeMapping;
		AssocVector<StringHash, std::any> m_properties{};
	};
}