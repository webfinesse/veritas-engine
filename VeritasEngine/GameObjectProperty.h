#ifndef H_I_GAME_OBJECT_PROPERTY
#define H_I_GAME_OBJECT_PROPERTY

#include <string>
#include <memory>
#include <utility>
#include <unordered_map>

#include "../VeritasEngineBase/GameObjectHandle.h"
#include "../VeritasEngineBase/SmallObject.h"

#include "DeserializerFactory.h"
#include "DeserializeMapping.h"

using namespace std;

namespace VeritasEngine
{
	template <typename T>
	class GameObjectProperty: public SmallObject<> {
	public:
		template<typename U, typename L>
		explicit GameObjectProperty(long propertyId, U&& name, L&& jsonTag)
			: m_propertyId{ propertyId }, m_name(std::forward<U>(name))
		{
			string tag(std::forward<L>(jsonTag));
			auto localThis = this;
			DeserializeMapping::Instance().Register(tag, [localThis](GameObjectHandle handle, JsonValue& values) -> void {
				localThis->Deserialize(handle, values);
			});
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

		void Deserialize(GameObjectHandle handle, JsonValue& values) {

			auto deserializer = DeserializerFactory<T>::GetDeserializer();

			m_properties.emplace(handle, make_shared<T>(deserializer(values)));
		}

	private:
		long m_propertyId;
		wstring m_name;
		unordered_map<GameObjectHandle, shared_ptr<T>> m_properties;
	};

	template <typename T>
	class GameObjectProperty<T*> : public SmallObject<> {
	public:
		template<typename U, typename L>
		explicit GameObjectProperty(long propertyId, U&& name, L&& jsonTag)
			: m_propertyId{ propertyId }, m_name(std::forward<U>(name))
		{
			string tag(std::forward<L>(jsonTag));
			auto localThis = this;
			DeserializeMapping::Instance().Register(tag, [localThis](GameObjectHandle handle, JsonValue& values) -> void {
				localThis->Deserialize(handle, values);
			});
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

		void Deserialize(GameObjectHandle handle, JsonValue& values) {

			auto deserializer = DeserializerFactory<T*>::GetDeserializer();

			m_properties.emplace(handle, deserializer(values));
		}

	private:
		long m_propertyId;
		wstring m_name;
		unordered_map<GameObjectHandle, T*> m_properties;
	};
}

#endif