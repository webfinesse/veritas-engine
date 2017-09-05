#include <fstream>
#include <string>

#include "ACPState.h"
#include "ExportMesh.h"
#include "../VeritasEngineBase/MeshInfo.h"
#include "../VeritasEngineBase/zip_file.hpp"
#include <boost/filesystem.hpp>
#include "MeshSerializer.h"

namespace fs = boost::filesystem;

void ProcessMesh(fs::path& path) 
{
	VeritasACP::ExportMesh exporter;
	auto meshInfo = exporter.Export(path);

	if (meshInfo != nullptr)
	{
		VeritasACP::MeshSerializer serializer;
		serializer.Serialize(*meshInfo, path);		
	}
}

void ProcessSubDirectory(fs::path& path)
{
	const static std::unordered_set<std::wstring> meshExtensionMap = { L".obj", L".dae", L".fbx" };

	for (auto& item : fs::directory_iterator(path))
	{
		auto itemPath = item.path();

		if (fs::is_directory(itemPath))
		{
			ProcessSubDirectory(itemPath);
		}
		else if (fs::is_regular_file(itemPath) && itemPath.has_extension())
		{
			auto extension = itemPath.extension();
			auto fileExtensionMapIter = meshExtensionMap.find(std::wstring(extension.c_str()));
			if (fileExtensionMapIter != meshExtensionMap.end())
			{
				ProcessMesh(itemPath);
			}
		}
	}
}

void ProcessBaseDirectory(fs::path& basePath)
{
	auto& state = VeritasACP::ACPState::Instance();

	for (auto& item : fs::directory_iterator(basePath))
	{
		if (fs::is_directory(item))
		{
			auto outputPath = item.path();
			ProcessSubDirectory(outputPath);
			
			if (state.GetAssetListSize() > 0)
			{
				zip_file zip;

				for (const auto& path : state.GetAssetList())
				{
					auto relative = fs::relative(path, outputPath);
					auto relativeString = relative.generic_string();

					zip.write(path.generic_string(), relative.generic_string());
				}

				auto copy = basePath;
				copy.append(outputPath.filename().generic_string());
				copy.replace_extension(L".zip");
				zip.save(copy.generic_string());

				state.ResetAssetList();
			}
		}
	}
}

int main(int argCount, char* args[])
{
	if (argCount > 1)
	{
		fs::path basePath(fs::current_path());

		if (argCount > 1)
		{
			basePath = fs::system_complete(args[1]);
		}
			
		if (fs::is_directory(basePath)) 
		{
			auto& state = VeritasACP::ACPState::Instance();
			state.Init(basePath);

			ProcessBaseDirectory(basePath);
		}
	}
}



