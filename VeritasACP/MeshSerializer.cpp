#include <boost/filesystem.hpp>
#include <cereal/archives/binary.hpp>

#include "../VeritasEngineBase/MeshInfo.h"
#include "../VeritasEngineBase/ResourceId.h"
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
	targetNode.m_jointIndex = node.m_jointIndex;

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
		result.emplace_back();
		auto& currentVertex = result.back();

		currentVertex.Position = vertex.Position;
		currentVertex.Normal = vertex.Normal;
		currentVertex.Tangent = vertex.Tangent;
		currentVertex.Binormal = vertex.Binormal;
		currentVertex.TextureCoordinates = vertex.TextureCoordinates;

		for (auto index = 0; index < MAX_JOINT_WEIGHTS; index++)
		{
			currentVertex.JointIndicies[index] = std::byte(0);
		}

		for (auto index = 0; index < vertex.JointIndicies.size(); index++)
		{
			currentVertex.JointIndicies[index] = vertex.JointIndicies[index];
		}
		

		std::size_t maxWeights = MAX_JOINT_WEIGHTS - 1;
		for (auto index = 0; index < maxWeights; index++)
		{
			currentVertex.JointWeights[index] = 0;
		}

		float cumulativeWeight = 0;

		auto length = std::min(vertex.JointWeights.size(), maxWeights);
		for (auto index = 0; index < length; index++)
		{
			currentVertex.JointWeights[index] = vertex.JointWeights[index];
			cumulativeWeight += currentVertex.JointWeights[index];
		}

		assert(cumulativeWeight <= 1.0005);				
	}

	return result;
}

template <class MeshType>
MeshType SerializeMeshInfo(VeritasACP::MeshExporterResult& meshInfo)
{
	MeshType mi;

	mi.m_subsets.reserve(meshInfo.m_subsets.size());
	mi.m_globalInverseTransform = meshInfo.m_globalInverseTransform;

	for (auto& subset : meshInfo.m_subsets)
	{
		VeritasEngine::SerializedMeshSubset newSubset;

		newSubset.m_vertexBaseIndex = subset.m_vertexBaseIndex;
		newSubset.m_vertexCount = subset.m_vertexCount;
		newSubset.m_indexBaseIndex = subset.m_indexBaseIndex;
		newSubset.m_indexCount = subset.m_indexCount;
		newSubset.m_materialId = std::move(subset.m_material);

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

	VeritasACP::ACPState::Instance().AddAsset(outputPath);

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

	const auto fullPath = SerializeArchive(skel, path, L".vesk");
	const auto parentPath = fs::path(fullPath).parent_path().parent_path();
	const auto relative = fs::relative(fullPath, parentPath);

	return VeritasEngine::ResourceId(relative.generic_string());
}

std::vector<VeritasEngine::Animation> SerializeAnimations(VeritasACP::MeshExporterResult& meshInfo)
{
	std::vector<VeritasEngine::Animation> result;

	result.reserve(meshInfo.m_animations.size());

	for(const auto& animation : meshInfo.m_animations)
	{
		result.emplace_back();
		auto& ani = result.back();

		auto& clipResult = animation.second;
		ani.HashedName = clipResult.m_hashedName;
		ani.Duration = clipResult.m_duration;

		ani.BoneInfo.reserve(clipResult.m_poses.size());

		for(const auto& boneInfo : clipResult.m_poses)
		{
			ani.BoneInfo.emplace_back();
			auto& info = ani.BoneInfo.back();

			info.BoneIndex = boneInfo.m_jointIndex;

			info.ScaleChannel.reserve(boneInfo.ScaleChannel.size());
			info.RotationChannel.reserve(boneInfo.RotationChannel.size());
			info.TranslationChannel.reserve(boneInfo.TranslationChannel.size());

			for(const auto scale : boneInfo.ScaleChannel)
			{
				info.ScaleChannel.emplace_back();
				info.ScaleChannel.back().Data = scale.m_data;
				info.ScaleChannel.back().Time = scale.m_time;
			}

			for (const auto rotation : boneInfo.RotationChannel)
			{
				info.RotationChannel.emplace_back();
				info.RotationChannel.back().Data = rotation.m_data;
				info.RotationChannel.back().Time = rotation.m_time;
			}

			for (const auto translation : boneInfo.TranslationChannel)
			{
				info.TranslationChannel.emplace_back();
				info.TranslationChannel.back().Data = translation.m_data;
				info.TranslationChannel.back().Time = translation.m_time;
			}
		}
	}

	return result;
}

void VeritasACP::MeshSerializer::Serialize(MeshExporterResult& meshInfo, fs::path& path) const
{
	VeritasEngine::MeshInfo mi;

	mi.m_subsets.reserve(meshInfo.m_subsets.size());

	if (meshInfo.m_animations.size() > 0)
	{
		auto result = SerializeMeshInfo<VeritasEngine::SkinnedMeshInfo>(meshInfo);
		result.m_verticies = SerializeVerticies<VeritasEngine::SkinnedMeshInfo::VertexType>(meshInfo.m_verticies);
		result.m_indicies = std::move(meshInfo.m_indicies);
		result.m_skeletonId = SerializeSkeleton(meshInfo, path);
		result.m_animations = SerializeAnimations(meshInfo);
		SerializeArchive(result, path, L".veam");
	}
	else
	{
		auto result = SerializeMeshInfo<VeritasEngine::MeshInfo>(meshInfo);
		result.m_verticies = SerializeVerticies<VeritasEngine::MeshInfo::VertexType>(meshInfo.m_verticies);
		result.m_indicies = std::move(meshInfo.m_indicies);
		SerializeArchive(result, path, L".vem");
	}
}
