#ifndef H_I_GAME_OBJECT_PROPERTY
#define H_I_GAME_OBJECT_PROPERTY

#include "../Includes/AssocVector/AssocVector.hpp"

#include <memory>

#include "../VeritasEngineBase/GameObjectHandle.h"
#include "SmallObject.h"

#include "DeserializerFactory.h"
#include "IDeserializeMapping.h"

using namespace std;

namespace VeritasEngine
{
	template <typename T>
	class GameObjectProperty: public SmallObject<> {
	public:
		explicit GameObjectProperty(const char* name, StringHash jsonTag)
			: m_propertyId{ Hash(name) }, m_name(name)
		{

		}

		bool operator==(const GameObjectProperty& other) const
		{
			return (m_propertyId == other.m_propertyId);
		}

		long GetPropertyId() const
		{
			return m_propertyId;
		}

		shared_ptr<T> GetProperty(const GameObjectHandle& gameObject)
		{
			auto query = m_properties.find(gameObject);

			if (query != m_properties.end())
			{
				return query->second;
			}

			return nullptr;
		}

		void SetProperty(GameObjectHandle& gameObject, T& value)
		{
			m_properties.emplace(gameObject, make_shared<T>(value));
		}

		void SetProperty(GameObjectHandle& gameObject, T&& value)
		{
			m_properties.emplace(gameObject, make_shared<T>(std::forward(value)));
		}

		void RemoveProperty(GameObjectHandle& gameObject)
		{
			m_properties.erase(gameObject);
		}

		void Clear()
		{
			m_properties.clear();
		}

		void Deserialize(Engine& engine, GameObjectHandle handle, JsonValue& values) {

			auto deserializer = DeserializerFactory<T>::GetDeserializer();

			m_properties.emplace(handle, make_shared<T>(deserializer(engine, values)));
		}

	private:
		StringHash m_propertyId;
		string m_name;
		AssocVector<GameObjectHandle, shared_ptr<T>> m_properties;
	};

	template <typename T>
	class GameObjectProperty<T*> : public SmallObject<> {
	public:
		explicit GameObjectProperty(const char* name, StringHash jsonTag)
			: m_propertyId{ Hash(name) }, m_name(name)
		{

		}

		bool operator==(const GameObjectProperty& other) const
		{
			return (m_propertyId == other.m_propertyId);
		}

		long GetPropertyId() const
		{
			return m_propertyId;
		}

		T* GetProperty(const GameObjectHandle& gameObject)
		{
			auto query = m_properties.find(gameObject);

			if (query != m_properties.end())
			{
				return query->second;
			}

			return nullptr;
		}

		void SetProperty(GameObjectHandle& gameObject, T* value)
		{
			m_properties.emplace(gameObject, value);
		}

		void RemoveProperty(GameObjectHandle& gameObject)
		{
			m_properties.erase(gameObject);
		}

		void Clear()
		{
			m_properties.clear();
		}

		void Deserialize(Engine& engine, GameObjectHandle handle, JsonValue& values) {

			auto deserializer = DeserializerFactory<T*>::GetDeserializer();

			m_properties.emplace(handle, deserializer(engine, values));
		}

	private:
		StringHash m_propertyId;
		string m_name;
		AssocVector<GameObjectHandle, T*> m_properties;
	};
}

#endif