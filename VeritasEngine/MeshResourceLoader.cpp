#include "MeshResourceLoader.h"

#include "../VeritasEngineBase/MeshInfo.h"
#include "MeshInstance.h"
#include "MeshSubset.h"
#include "MeshNode.h"
#include "IRenderingServices.h"
#include "VertexBufferManager.h"
#include "IIndexBuffer.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"

#include <memory>

#include "../VeritasEngineBase/ResourceHandle.h"
#include "ResourceManager.h"

struct VeritasEngine::MeshResourceLoader::Impl
{
	Impl(std::shared_ptr<IRenderingServices> renderingServices)
		: m_renderingServices { renderingServices }
	{
		
	}

	std::shared_ptr<IRenderingServices> m_renderingServices;
};

VeritasEngine::MeshResourceLoader::MeshResourceLoader(std::shared_ptr<IRenderingServices> renderingServices)
	: m_impl(std::make_unique<Impl>(renderingServices))
{
}

VeritasEngine::MeshResourceLoader::~MeshResourceLoader() = default;

const char* VeritasEngine::MeshResourceLoader::GetExtension() const
{
	return ".vem";
}

void VeritasEngine::MeshResourceLoader::LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	cereal::BinaryInputArchive archive(data);

	MeshInfo info;

	archive(info);

	MeshInstance mesh{};

	for(auto& serializedSubset : info.m_subsets)
	{
		auto& instanceSubset = mesh.CreateSubset();

		auto vertexBuffer = m_impl->m_renderingServices->GetVertexBufferManager().GetBuffer(info.m_vertexType);

		instanceSubset.SetVertices(vertexBuffer, reinterpret_cast<unsigned char*>(&serializedSubset.m_verticies[0]), serializedSubset.m_verticies.size());

		if (serializedSubset.m_faces.size() > 0)
		{
			instanceSubset.SetIndicies(&m_impl->m_renderingServices->GetIndexBuffer(), &serializedSubset.m_faces[0], serializedSubset.m_faces.size());
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
