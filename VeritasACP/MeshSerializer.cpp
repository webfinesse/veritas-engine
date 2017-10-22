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
		result.emplace_back();
		auto& currentVertex = result.back();

		currentVertex.Position = vertex.Position;
		currentVertex.Normal = vertex.Normal;
		currentVertex.Tangent = vertex.Tangent;
		currentVertex.Binormal = vertex.Binormal;
		currentVertex.TextureCoordinates = vertex.TextureCoordinates;

		auto index = 0;

		if (vertex.JointIndicies.size() > 0)
		{
			for (; index < vertex.JointIndicies.size(); index++)
			{
				currentVertex.JointIndicies[index] = vertex.JointIndicies[index];
			}
		}

		for(; index < MAX_JOINT_WEIGHTS; index++)
		{
			currentVertex.JointIndicies[index] = static_cast<std::byte>(-1);
		}

		index = 0;
		
		if (vertex.JointWeights.size() > 0)
		{
			for (; index < vertex.JointWeights.size() - 1; index++)
			{
				currentVertex.JointWeights[index] = vertex.JointWeights[index];
			}
		}

		for (; index < MAX_JOINT_WEIGHTS - 1; index++)
		{
			currentVertex.JointWeights[index] = 0;
		}
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

	auto fullPath = SerializeArchive(skel, path, L".vesk");
	auto parentPath = fs::path(fullPath).parent_path().parent_path();
	auto relative = fs::relative(fullPath, parentPath);

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

			info.Keyframes.reserve(boneInfo.m_keyframes.size());

			for(const auto& keyframe : boneInfo.m_keyframes)
			{
				info.Keyframes.emplace_back();
				auto& currentKeyFrame = info.Keyframes.back();

				currentKeyFrame.TimeSample = keyframe.m_timeSample;
				currentKeyFrame.Scale = keyframe.m_scale;
				currentKeyFrame.Rotation = keyframe.m_rotation;
				currentKeyFrame.Translation = keyframe.m_translation;
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
		result.m_skeletonId = SerializeSkeleton(meshInfo, path);
		result.m_animations = SerializeAnimations(meshInfo);
		SerializeArchive(result, path, L".veam");
	}
	else
	{
		auto result = SerializeMeshInfo<VeritasEngine::MeshInfo>(meshInfo);
		SerializeArchive(result, path, L".vem");
	}
}
