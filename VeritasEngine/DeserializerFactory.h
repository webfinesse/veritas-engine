#pragma once

#include <algorithm>
#include <vector>

#include "MeshInstance.h"
#include "IRenderingServices.h"
#include "VertexBufferManager.h"
#include "IVertexBuffer.h"
#include "IAnimationManager.h"

#include "../VeritasEngineBase/Light.h"
#include "../VeritasEngineBase/MathTypes.h"

#include "JsonValue.h"
#include "MeshSubset.h"
#include "SceneNodeType.h"
#include "Vertex.h"
#include "Engine.h"
#include "ResourceManager.h"
#include "IIndexBuffer.h"
#include "IJobManager.h"

namespace VeritasEngine
{
	template<typename T>
	class DeserializerFactory
	{
	public:
		using FUNCTIONTYPE = T(*)(Engine& engine, JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			static_assert(true, "A deserializer was not found for the specified type");
			return nullptr;
		}
	};

	class DeserializerFactoryHelper
	{
	public:
		template <typename T>
		static T GetDeserializedValue(Engine& engine, JsonValue& values, const std::string jsonTag, T defaultValue)
		{
			auto const& value = values.find(jsonTag);

			if (value != values.not_found())
			{
				return DeserializerFactory<T>::GetDeserializer()(engine, value->second);
			}
			else
			{
				return defaultValue;
			}
		}

		template <typename T>
		static T GetValue(JsonValue& values, const std::string jsonTag, T defaultValue)
		{
			auto const& value = values.find(jsonTag);

			if (value != values.not_found())
			{
				return value->second.get_value<T>();
			}
			else
			{
				return defaultValue;
			}
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory < VeritasEngine::Matrix4x4 >
	{
	public:
		using FUNCTIONTYPE = VeritasEngine::Matrix4x4(*)(Engine& engine, JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static VeritasEngine::Matrix4x4 Deserialize(Engine& engine, JsonValue& values)
		{
			Matrix4x4 m;

			m[0][0] = values.find("_11")->second.get_value<float>();
			m[0][1] = values.find("_12")->second.get_value<float>();
			m[0][2] = values.find("_13")->second.get_value<float>();
			m[0][3] = values.find("_14")->second.get_value<float>();

			m[1][0] = values.find("_21")->second.get_value<float>();
			m[1][1] = values.find("_22")->second.get_value<float>();
			m[1][2] = values.find("_23")->second.get_value<float>();
			m[1][3] = values.find("_24")->second.get_value<float>();

			m[2][0] = values.find("_31")->second.get_value<float>();
			m[2][1] = values.find("_32")->second.get_value<float>();
			m[2][2] = values.find("_33")->second.get_value<float>();
			m[2][3] = values.find("_34")->second.get_value<float>();

			m[3][0] = values.find("_41")->second.get_value<float>();
			m[3][1] = values.find("_42")->second.get_value<float>();
			m[3][2] = values.find("_43")->second.get_value<float>();
			m[3][3] = values.find("_44")->second.get_value<float>();

			return m;
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory < VeritasEngine::SceneNodeType >
	{
	public:
		using FUNCTIONTYPE = SceneNodeType(*)(Engine& engine, JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static SceneNodeType Deserialize(Engine& engine, JsonValue& values)
		{
			auto returnValue = SceneNodeType::None;

			auto nodeTypeName = values.get_value<std::string>();
			std::transform(nodeTypeName.begin(), nodeTypeName.end(), nodeTypeName.begin(), toupper);

			if (nodeTypeName == "MESH")
			{
				returnValue = SceneNodeType::Mesh;
			}
			else if (nodeTypeName == "ANIMATEDRESOURCEDMESH")
			{
				returnValue = SceneNodeType::AnimatedResourcedMesh;
			}
			else if (nodeTypeName == "RESOURCEDMESH")
			{
				returnValue = SceneNodeType::ResourcedMesh;
			}
			else if (nodeTypeName == "CAMERA")
			{
				returnValue = SceneNodeType::Camera;
			}
			else if (nodeTypeName == "LIGHT")
			{
				returnValue = SceneNodeType::Light;
			}

			return returnValue;
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory < VeritasEngine::Float3 >
	{
	public:
		using FUNCTIONTYPE = VeritasEngine::Float3(*)(Engine& engine, JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static VeritasEngine::Float3 Deserialize(Engine& engine, JsonValue& values)
		{
			VeritasEngine::Float3 returnValue(values.find("x")->second.get_value<float>(), values.find("y")->second.get_value<float>(), values.find("z")->second.get_value<float>());

			return returnValue;
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory < VeritasEngine::Float4 >
	{
	public:
		using FUNCTIONTYPE = VeritasEngine::Float4(*)(Engine& engine, JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static VeritasEngine::Float4 Deserialize(Engine& engine, JsonValue& values)
		{
			VeritasEngine::Float4 returnValue(values.find("x")->second.get_value<float>(), values.find("y")->second.get_value<float>(), values.find("z")->second.get_value<float>(), values.find("w")->second.get_value<float>());

			return returnValue;
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory< VeritasEngine::Light >
	{
	public:
		using FUNCTIONTYPE = Light(*)(Engine& engine, JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static VeritasEngine::Light Deserialize(Engine& engine, JsonValue& values)
		{
			VeritasEngine::Light returnValue;

			returnValue.Position = DeserializerFactoryHelper::GetDeserializedValue(engine, values, "position", Float4());
			returnValue.Direction = DeserializerFactoryHelper::GetDeserializedValue(engine, values, "direction", Float4());
			returnValue.Color = DeserializerFactoryHelper::GetDeserializedValue(engine, values, "color", Float4());
			returnValue.SpotAngle = DeserializerFactoryHelper::GetValue(values, "spotAngle", 0.0f);
			returnValue.ConstantAttenuation = DeserializerFactoryHelper::GetValue(values, "constantattenuation", 0.0f);
			returnValue.QuadraticAttenuation = DeserializerFactoryHelper::GetValue(values, "quadraticattenuation", 0.0f);
			returnValue.Enabled = DeserializerFactoryHelper::GetValue(values, "enabled", 1);

			auto type = LightType::Directional;

			const auto& typeValue = values.find("type");
			if (typeValue != values.not_found())
			{
				auto typeString = typeValue->second.get_value<std::string>();

				if (typeString == "spotlight")
				{
					type = LightType::Spotlight;
				}
				else if (typeString == "point")
				{
					type = LightType::Point;
				}
			}

			returnValue.Type = type;

			return returnValue;
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory < VeritasEngine::ResourceHandle >
	{
	public:
		using FUNCTIONTYPE = ResourceHandle(*)(Engine& engine, JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static ResourceHandle Deserialize(Engine& engine, JsonValue& values)
		{
			const auto resourceId = values.get_value<std::string>();
			return engine.GetResourceManager().LoadResource(resourceId);
		}
	};

	class AnimationDeserializerFactory
	{
	public:
		using FUNCTIONTYPE = void(*)(Engine& engine, GameObjectHandle handle, JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

		static void Deserialize(Engine& engine, GameObjectHandle handle, JsonValue& values)
		{
			const std::string animationName = DeserializerFactoryHelper::GetValue(values, "animationname", std::string(""));
			const bool isLooped = DeserializerFactoryHelper::GetValue(values, "islooped", false);
			const float timeScale = DeserializerFactoryHelper::GetValue(values, "timeScale", 1.0f);

			engine.GetAnimationManager().AddAnimaton(handle, Hash(animationName), isLooped, timeScale);
		}
	};
}