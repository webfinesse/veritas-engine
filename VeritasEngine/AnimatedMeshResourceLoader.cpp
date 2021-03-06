#include "AnimatedMeshResourceLoader.h"

#include "../VeritasEngineBase/MeshInfo.h"
#include "MeshInstance.h"
#include "MeshSubset.h"
#include "MeshNode.h"
#include "RenderingServices.h"
#include "VertexBufferManager.h"
#include "IIndexBuffer.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"

#include <memory>

#include "ResourceManager.h"
#include "ResourceData.h"

constexpr char extension[] = ".veam";

struct VeritasEngine::AnimatedMeshResourceLoader::Impl
{
	Impl(std::shared_ptr<IRenderingServices>&& renderingServices)
		: m_renderingServices { std::move(renderingServices) }
	{
		
	}

	std::shared_ptr<IRenderingServices> m_renderingServices{};
};

VeritasEngine::AnimatedMeshResourceLoader::AnimatedMeshResourceLoader(std::shared_ptr<IRenderingServices> renderingServices)
	: m_impl(std::make_unique<Impl>(std::move(renderingServices)))
{

}

const char* VeritasEngine::AnimatedMeshResourceLoader::GetExtension() const
{
	return extension;
}

VeritasEngine::StringHash VeritasEngine::AnimatedMeshResourceLoader::GetExtensionHash() const
{
	constexpr auto hash = CompileTimeHash(extension);
	return hash;
}

void VeritasEngine::AnimatedMeshResourceLoader::LoadResource(IResourceManager& manager, Job* parentJob, std::istream& data, ResourceData& handle)
{
	cereal::BinaryInputArchive archive(data);

	SkinnedMeshInfo info;

	archive(info);

	MeshInstance mesh{};

	const auto vertexBuffer = m_impl->m_renderingServices->GetVertexBufferManager().GetBuffer(info.m_vertexType);
	mesh.SetVertices(vertexBuffer, reinterpret_cast<unsigned char*>(&info.m_verticies[0]), info.m_verticies.size());

	auto& indexBuffer = m_impl->m_renderingServices->GetIndexBuffer();
	mesh.SetIndicies(&indexBuffer, &info.m_indicies[0], info.m_indicies.size());

	mesh.SetGlobalInverseTransform(info.m_globalInverseTransform);	

	mesh.SetAnimations(info.m_animations);	

	auto& rootNode = mesh.GetRootNode();

	rootNode.SetTransform(info.m_root.m_transform);
	rootNode.SetMeshIndices(info.m_root.meshIndicies);
	rootNode.SetJointIndex(info.m_root.m_jointIndex);

	for (auto& child : info.m_root.m_children)
	{
		rootNode.AddChild(child);
	}

	for (const auto& serializedSubset : info.m_subsets)
	{
		auto& subset = mesh.CreateSubset(serializedSubset);
		subset.SetMaterial(manager.LoadResource(serializedSubset.m_materialId, parentJob));
	}
	
	mesh.SetSkeleton(manager.LoadResource(info.m_skeletonId, parentJob));

	handle.SetData(std::move(mesh));
}


