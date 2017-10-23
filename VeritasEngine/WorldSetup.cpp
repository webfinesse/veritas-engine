#include "WorldSetup.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "IDeserializeMapping.h"
#include "Engine.h"
#include "Scene.h"
#include "RenderingServices.h"

struct VeritasEngine::WorldSetup::Impl
{
	Impl(std::shared_ptr<IDeserializeMapping> deserializeMapping)
		: m_deserializeMapping { deserializeMapping }
	{
		
	}

	void LoadFile(const char* filename)
	{
		boost::property_tree::ptree jsonRoot;
		read_json(std::string(filename), jsonRoot);

		auto& scene = m_engine->GetRenderingServices().GetScene();

		for (const auto& rootItem : jsonRoot)
		{
			auto id = rootItem.second.find("Id")->second.get_value<int>();

			for (const auto& sceneItem : rootItem.second)
			{
				if (sceneItem.first != "Id")
				{
					auto deserializer = m_deserializeMapping->GetDeserializer(Hash(sceneItem.first.c_str()));
					assert(deserializer != nullptr);
					(*deserializer)(*m_engine, id, sceneItem.second);
				}
			}

			scene.Add(id);
		}
	}

	Engine* m_engine { nullptr };
	std::shared_ptr<IDeserializeMapping> m_deserializeMapping;
};

VeritasEngine::WorldSetup::WorldSetup(std::shared_ptr<IDeserializeMapping> deserializeMapping)
	: m_impl(std::make_unique<Impl>(deserializeMapping))
{
}

VeritasEngine::WorldSetup::WorldSetup(WorldSetup&& other) noexcept
	: m_impl{ std::move(other.m_impl) }
{
}

VeritasEngine::WorldSetup& VeritasEngine::WorldSetup::operator=(WorldSetup&& other) noexcept
{
	if(this != &other)
	{
		this->m_impl = std::move(other.m_impl);
	}

	return *this;
}

VeritasEngine::WorldSetup::~WorldSetup() = default;

void VeritasEngine::WorldSetup::Init(Engine& engine)
{
	m_impl->m_engine = &engine;
}

void VeritasEngine::WorldSetup::LoadFile(const char* filename)
{	
	m_impl->LoadFile(filename);
}
