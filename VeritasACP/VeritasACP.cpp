#include <fstream>
#include <string>

#include "ACPState.h"
#include "ExportMesh.h"
#include "../VeritasEngineBase/MeshInfo.h"

#include "../Includes/cereal-1.1.2/include/cereal/archives/binary.hpp"
#include "../VeritasEngineBase/zip_file.hpp"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

void SerializeMeshNodes(VeritasACP::MeshExporterNode& node, VeritasEngine::SerializedMeshNode& targetNode)
{
	targetNode.m_transform = node.m_transform;
	targetNode.meshIndicies = node.meshIndicies;
	
	for (auto& childNode : node.m_children)
	{
		targetNode.m_children.emplace_back();

		auto& targetChildNode = targetNode.m_children.back();

		SerializeMeshNodes(childNode, targetChildNode);
	}
}

void ProcessMesh(fs::path& path) 
{
	VeritasACP::ExportMesh exporter;
	auto meshInfo = exporter.Export(path);

	if (meshInfo != nullptr)
	{
		VeritasEngine::MeshInfo mi;

		mi.m_subsets.reserve(meshInfo->m_subsets.size());

		for (auto& subset : meshInfo->m_subsets)
		{
			VeritasEngine::SerializedMeshSubset newSubset;

			//newSubset.m_verticies = move(subset.m_vertices);
			newSubset.m_faces = move(subset.m_faces);
			newSubset.m_materialId = move(subset.m_material);

			mi.m_subsets.emplace_back(newSubset);
		}

		SerializeMeshNodes(meshInfo->m_root, mi.m_root);

		mi.m_skeletonId = {};

		auto& outputPath = path.replace_extension(L".vem");

		std::ofstream file(outputPath.generic_string(), std::ios::binary);
		cereal::BinaryOutputArchive archive(file);

		VeritasACP::ACPState::Instance().GetAssetList().push_back(outputPath);

		archive(mi);
	}
}

void ProcessSubDirectory(fs::path& path)
{
	const static std::unordered_set<std::wstring> meshExtensionMap = { /* L".obj" */ L".dae" };

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
			
			if (state.GetAssetList().size() > 0)
			{
				zip_file zip;

				for (auto& path : state.GetAssetList())
				{
					auto relative = fs::relative(path, outputPath);
					auto relativeString = relative.generic_string();

					zip.write(path.generic_string(), relative.generic_string());
				}

				auto copy = basePath;
				copy.append(outputPath.filename().generic_string());
				copy.replace_extension(L".zip");
				zip.save(copy.generic_string());

				state.GetAssetList().resize(0);
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



