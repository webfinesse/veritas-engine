#include "WorldSetup.h"

#include "..\Includes\rapidjson\include/rapidjson/document.h"
#include "..\Includes\rapidjson/include/rapidjson/filereadstream.h"

#include "DeserializeMapping.h"
#include "Engine.h"
#include "Scene.h"
#include "RenderingServices.h"

void VeritasEngine::WorldSetup::LoadFile(const char* filename)
{
	FILE* pFile = nullptr;
	auto error = fopen_s(&pFile, filename, "rb");

	char buffer[10000];

	if (pFile != nullptr)
	{
		
		rapidjson::FileReadStream worldSetup(pFile, buffer, sizeof(buffer));

		rapidjson::Document d;
		d.ParseStream(worldSetup);

		auto& scene = Engine::Instance().GetRenderingServices().GetScene();

		auto& mapping = DeserializeMapping::Instance();

		for (rapidjson::SizeType i = 0; i < d.Size(); i++)
		{
			const rapidjson::Value& item = d[i];
			auto id = item["Id"].GetInt();

			for (auto memberIter = item.MemberBegin(); memberIter != item.MemberEnd(); ++memberIter)
			{
				auto memberName = memberIter->name.GetString();
				if (strncmp(memberName, "Id", 2) != 0)
				{
					auto length = memberIter->name.GetStringLength();
					string copy(memberName, length);
					auto deserializer = mapping.GetDeserializer(copy);
					(*deserializer)(id, memberIter->value);
				}
			}

			scene.Add(id);
		}

		fclose(pFile);
	}
	else
	{
		strerror_s(buffer, sizeof(buffer),error);
	}
}