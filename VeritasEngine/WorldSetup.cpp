#include "WorldSetup.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "DeserializeMapping.h"
#include "Engine.h"
#include "Scene.h"
#include "RenderingServices.h"

void VeritasEngine::WorldSetup::LoadFile(const char* filename)
{	
		boost::property_tree::ptree jsonRoot;
		read_json(std::string(filename), jsonRoot);

		auto& scene = Engine::Instance().GetRenderingServices().GetScene();

		auto& mapping = DeserializeMapping::Instance();

		for (const auto& rootItem : jsonRoot)
		{
			auto id = rootItem.second.find("Id")->second.get_value<int>();
			
			for (const auto& sceneItem : rootItem.second)
			{
				if (strncmp(sceneItem.first.c_str(), "Id", 2) != 0)
				{
					auto deserializer = mapping.GetDeserializer(sceneItem.first);
					(*deserializer)(id, sceneItem.second);
				}
			}

			scene.Add(id);
		}
}
