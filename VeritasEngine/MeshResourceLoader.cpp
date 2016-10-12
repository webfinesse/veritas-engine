#include "MeshResourceLoader.h"

#include "../VeritasEngineBase/MeshInfo.h"
#include "MeshInstance.h"
#include "MeshSubset.h"
#include "MeshNode.h"

#include "../Includes/cereal-1.1.2/include/cereal/cereal.hpp"
#include "../Includes/cereal-1.1.2/include/cereal/archives/binary.hpp"

#include <memory>
#include "../Includes/gsl/include/gsl.h"

#include "../VeritasEngineBase/ResourceHandle.h"
#include "ResourceManager.h"

VeritasEngine::MeshResourceLoader::~MeshResourceLoader() = default;

void VeritasEngine::MeshResourceLoader::LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	cereal::BinaryInputArchive archive(data);

	MeshInfo info;

	archive(info);

	MeshInstance mesh{};

	for(auto& serializedSubset : info.m_subsets)
	{
		auto& instanceSubset = mesh.CreateSubset();

		instanceSubset.SetVertices(Vertex::Type, reinterpret_cast<gsl::byte*>(&serializedSubset.m_verticies[0]), serializedSubset.m_verticies.size());

		if (serializedSubset.m_faces.size() > 0)
		{
			instanceSubset.SetIndicies(&serializedSubset.m_faces[0], serializedSubset.m_faces.size());
		}

		auto material = manager.GetResource(serializedSubset.m_materialId);

		instanceSubset.SetMaterial(material);
	}

	auto& rootNode = mesh.GetRootNode();

	rootNode.SetTransform(info.m_root.m_transform);
	rootNode.SetMeshIndices(info.m_root.meshIndicies);


	for (auto& child : info.m_root.m_children)
	{
		rootNode.AddChild({ child });
	}

	handle.SetData(std::move(mesh));
}
