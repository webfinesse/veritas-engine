#include <boost/filesystem.hpp>
#include <cereal/archives/binary.hpp>

#include "../VeritasEngineBase/MeshInfo.h"
#include "../VeritasEngineBase/ResourceId.h"
#include "../VeritasEngine/SmallObject.h"
#include "../VeritasEngine/Skeleton.h"
#include "../VeritasEngine/SkeletonJoint.h"

#include "MeshExporterVertex.h"
#include "MeshExporterResult.h"
#include "ACPState.h"

#include "MeshSerializer.h"


VeritasACP::MeshSerializer::MeshSerializer()
{
}

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

template <class VertexType>
std::vector<VertexType> SerializeVerticies(std::vector<VeritasACP::MeshExporterVertex>& verticies)
{
	static_assert(0, "Could not find a proper template overload for SerializeVerticies");
	std::vector<VertexType> result;

	return result;
}

template <>
std::vector<VeritasEngine::Vertex> SerializeVerticies(std::vector<VeritasACP::MeshExporterVertex>& verticies)
{
	std::vector<VeritasEngine::Vertex> result;

	for (const auto& vertex : verticies)
	{
		result.emplace_back();
		auto& currentVertex = result.back();

		currentVertex.Position = vertex.Position;
		currentVertex.Normal = vertex.Normal;
		currentVertex.Tangent = vertex.Tangent;
		currentVertex.Binormal = vertex.Binormal;
		currentVertex.TextureCoordinates = vertex.TextureCoordinates;
	}

	return result;
}

template <>
std::vector<VeritasEngine::SkinnedVertex> SerializeVerticies(std::vector<VeritasACP::MeshExporterVertex>& verticies)
{
	std::vector<VeritasEngine::SkinnedVertex> result;

	for (const auto& vertex : verticies)
	{
		// the commented out code blocks are assuming there are at max 4 joints. Right now we support an arbitrary number of joints
		//assert(vertex.JointIndicies.size() <= 4);
		//assert(vertex.JointWeights.size() <= 4);

		result.emplace_back();
		auto& currentVertex = result.back();

		currentVertex.Position = vertex.Position;
		currentVertex.Normal = vertex.Normal;
		currentVertex.TextureCoordinates = vertex.TextureCoordinates;
		currentVertex.JointIndicies = vertex.JointIndicies;
		currentVertex.JointWeights = vertex.JointWeights;

		/*int currentIndex = 0;
		for(auto index: vertex.JointIndicies)
		{
		currentVertex.JointIndicies[currentIndex] = index;
		currentIndex++;
		}

		for(; currentIndex < 4; currentIndex++)
		{
		currentVertex.JointIndicies[currentIndex] = -1;
		}

		VeritasEngine::Float3 weights{0};

		currentIndex = 0;
		for (auto weight : vertex.JointWeights)
		{
		weights[currentIndex] = weight;
		currentIndex++;
		}*/
	}

	return result;
}

template <class MeshType>
MeshType SerializeMeshInfo(VeritasACP::MeshExporterResult& meshInfo)
{
	MeshType mi;

	mi.m_subsets.reserve(meshInfo.m_subsets.size());

	for (auto& subset : meshInfo.m_subsets)
	{
		typename MeshType::SubsetType newSubset;

		newSubset.m_verticies = SerializeVerticies<typename MeshType::VertexType>(subset.m_vertices);
		newSubset.m_faces = move(subset.m_faces);
		newSubset.m_materialId = move(subset.m_material);

		mi.m_subsets.emplace_back(newSubset);
	}

	SerializeMeshNodes(meshInfo.m_root, mi.m_root);

	return mi;
}

template <class ArchiveType>
VeritasEngine::ResourceId SerializeArchive(ArchiveType& archiveData, fs::path& path, wchar_t* extension)
{
	auto& outputPath = path.replace_extension(extension);

	std::ofstream file(outputPath.generic_string(), std::ios::binary);
	cereal::BinaryOutputArchive archive(file);

	VeritasACP::ACPState::Instance().GetAssetList().push_back(outputPath);

	archive(archiveData);

	return VeritasEngine::ResourceId(outputPath.generic_string());
}

VeritasEngine::ResourceId SerializeSkeleton(VeritasACP::MeshExporterResult& meshInfo, fs::path& path)
{
	VeritasEngine::Skeleton skel;

	skel.Joints.reserve(meshInfo.m_skeleton.Joints.size());

	for(const auto& exporterJoint : meshInfo.m_skeleton.Joints)
	{
		VeritasEngine::SkeletonJoint joint;
		joint.Name = exporterJoint.Name;
		joint.InverseBindPose = exporterJoint.InverseBindPose;
		joint.ParentIndex = static_cast<int>(exporterJoint.ParentIndex);

		skel.Joints.emplace_back(joint);
	}

	return SerializeArchive(skel, path, L".vesk");
}

void VeritasACP::MeshSerializer::Serialize(MeshExporterResult& meshInfo, fs::path& path) const
{
	VeritasEngine::MeshInfo mi;

	mi.m_subsets.reserve(meshInfo.m_subsets.size());

	if (meshInfo.m_animations.size() > 0)
	{
		auto result = SerializeMeshInfo<VeritasEngine::SkinnedMeshInfo>(meshInfo);
		result.m_skeletonId = SerializeSkeleton(meshInfo, path);
		SerializeArchive(result, path, L".vem");
	}
	else
	{
		auto result = SerializeMeshInfo<VeritasEngine::MeshInfo>(meshInfo);
		SerializeArchive(result, path, L".vem");
	}
}
