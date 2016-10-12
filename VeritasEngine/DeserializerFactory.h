#ifndef H_DESERIALIZERFACTORY
#define H_DESERIALIZERFACTORY

#include <string>
#include <algorithm>
#include <vector>

#include "../Includes/gsl/include/gsl.h"

#include "MeshInstance.h"
#include "MeshNode.h"

#include "../VeritasEngineBase/Light.h"
#include "../VeritasEngineBase/MathTypes.h"

#include "RapidJsonHelper.h"
#include "MeshSubset.h"
#include "SceneNodeType.h"
#include "Vertex.h"
#include "Engine.h"
#include "ResourceManager.h"


namespace VeritasEngine
{
	template<typename T>
	class DeserializerFactory
	{
	public:
		using FUNCTIONTYPE = T(*)(JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			static_assert(true, "A deserializer was not found for the specified type");
			return nullptr;
		}

	};

	template<>
	class VeritasEngine::DeserializerFactory < VeritasEngine::Matrix4x4 >
	{
	public:
		using FUNCTIONTYPE = VeritasEngine::Matrix4x4(*)(JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static VeritasEngine::Matrix4x4 Deserialize(JsonValue& values)
		{
			Matrix4x4 m;

			m[0][0] = static_cast<float>(values["_11"].GetDouble());
			m[0][1] = static_cast<float>(values["_12"].GetDouble());
			m[0][2] = static_cast<float>(values["_13"].GetDouble());
			m[0][3] = static_cast<float>(values["_14"].GetDouble());

			m[1][0] = static_cast<float>(values["_21"].GetDouble());
			m[1][1] = static_cast<float>(values["_22"].GetDouble());
			m[1][2] = static_cast<float>(values["_23"].GetDouble());
			m[1][3] = static_cast<float>(values["_24"].GetDouble());

			m[2][0] = static_cast<float>(values["_31"].GetDouble());
			m[2][1] = static_cast<float>(values["_32"].GetDouble());
			m[2][2] = static_cast<float>(values["_33"].GetDouble());
			m[2][3] = static_cast<float>(values["_34"].GetDouble());

			m[3][0] = static_cast<float>(values["_41"].GetDouble());
			m[3][1] = static_cast<float>(values["_42"].GetDouble());
			m[3][2] = static_cast<float>(values["_43"].GetDouble());
			m[3][3] = static_cast<float>(values["_44"].GetDouble());

			return m;
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory < VeritasEngine::MeshInstance >
	{
	public:
		using FUNCTIONTYPE = MeshInstance(*)(JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static MeshInstance Deserialize(JsonValue& values)
		{
			auto& vertexFormat = values["VertexFormat"];
			MeshInstance m{};

			auto format = vertexFormat.GetUint();
			std::vector<Vertex> verticies;
			
			auto& verticiesArray = values["Verticies"];
			for (rapidjson::SizeType indexIter = 0; indexIter < verticiesArray.Size(); ++indexIter)
			{
				VeritasEngine::Vertex v;
				auto& vertex = verticiesArray[indexIter];
				v.Position[0] = static_cast<float>(vertex["x"].GetDouble());
				v.Position[1] = static_cast<float>(vertex["y"].GetDouble());
				v.Position[2] = static_cast<float>(vertex["z"].GetDouble());

				v.Normal[0] = static_cast<float>(vertex["nx"].GetDouble());
				v.Normal[1] = static_cast<float>(vertex["ny"].GetDouble());
				v.Normal[2] = static_cast<float>(vertex["nz"].GetDouble());

				verticies.emplace_back(std::move(v));
			}

			auto& indiciesArray = values["Subsets"];

			for (auto subsetIter = indiciesArray.Begin(); subsetIter != indiciesArray.End(); ++subsetIter)
			{
				auto& subset = m.CreateSubset();

				subset.SetVertices(format, reinterpret_cast<gsl::byte*>(&verticies[0]), verticies.size());

				std::vector<unsigned int> indicies;

				for (rapidjson::SizeType indexIter = 0; indexIter < subsetIter->Size(); ++indexIter)
				{
					auto value = (*subsetIter)[indexIter].GetUint();
					indicies.push_back(value);
				}

				subset.SetIndicies(&indicies[0], indicies.size());
			}

			return m;
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory < VeritasEngine::SceneNodeType >
	{
	public:
		using FUNCTIONTYPE = SceneNodeType(*)(JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static SceneNodeType Deserialize(JsonValue& values)
		{
			auto returnValue = SceneNodeType::None;

			auto nodeTypeName = std::string(values.GetString(), values.GetStringLength());
			std::transform(nodeTypeName.begin(), nodeTypeName.end(), nodeTypeName.begin(), toupper);

			if (nodeTypeName == "MESH")
			{
				returnValue = SceneNodeType::Mesh;
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
		using FUNCTIONTYPE = VeritasEngine::Float3(*)(JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static VeritasEngine::Float3 Deserialize(JsonValue& values)
		{
			VeritasEngine::Float3 returnValue(static_cast<float>(values["x"].GetDouble()), static_cast<float>(values["y"].GetDouble()), static_cast<float>(values["z"].GetDouble()));

			return returnValue;
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory < VeritasEngine::Float4 >
	{
	public:
		using FUNCTIONTYPE = VeritasEngine::Float4(*)(JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static VeritasEngine::Float4 Deserialize(JsonValue& values)
		{
			VeritasEngine::Float4 returnValue(static_cast<float>(values["x"].GetDouble()), static_cast<float>(values["y"].GetDouble()), static_cast<float>(values["z"].GetDouble()), static_cast<float>(values["w"].GetDouble()));

			return returnValue;
		}
	};

	template<>
	class VeritasEngine::DeserializerFactory< VeritasEngine::Light >
	{
	public:
		using FUNCTIONTYPE = Light(*)(JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static VeritasEngine::Light Deserialize(JsonValue& values)
		{
			VeritasEngine::Light returnValue;

			returnValue.Position = values.HasMember("position") ? (DeserializerFactory<VeritasEngine::Float4>::GetDeserializer())(values["position"]) : Float4();
			returnValue.Direction = values.HasMember("direction") ? (DeserializerFactory<VeritasEngine::Float4>::GetDeserializer())(values["direction"]) : Float4();
			returnValue.Color = values.HasMember("color") ? (DeserializerFactory<VeritasEngine::Float4>::GetDeserializer())(values["color"]) : Float4();
			returnValue.SpotAngle = values.HasMember("spotangle") ? static_cast<float>(values["spotangle"].GetDouble()) : 0.0f;
			returnValue.ConstantAttenuation = values.HasMember("constantattenuation") ? static_cast<float>(values["constantattenuation"].GetDouble()) : 0.0f;
			returnValue.QuadraticAttenuation = values.HasMember("quadraticattenuation") ? static_cast<float>(values["quadraticattenuation"].GetDouble()) : 0.0f;
			returnValue.Enabled = returnValue.SpotAngle = values.HasMember("enabled") ? values["enabled"].GetInt() : 1;

			auto type = LightType::Directional;

			if (values.HasMember("type"))
			{
				auto typeString = values["type"].GetString();

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
	class VeritasEngine::DeserializerFactory < VeritasEngine::ResourceHandle* >
	{
	public:
		using FUNCTIONTYPE = ResourceHandle*(*)(JsonValue& values);

		static FUNCTIONTYPE GetDeserializer()
		{
			return &Deserialize;
		}

	private:
		static ResourceHandle* Deserialize(JsonValue& values)
		{
			auto resourceId = values.GetString();
			auto resource = Engine::Instance().GetResourceManager().GetResource(resourceId);

			return resource;
		}
	};
}


#endif