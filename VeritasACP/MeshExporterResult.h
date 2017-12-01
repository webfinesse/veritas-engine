#ifndef H_MESHEXPORTERRESULT
#define H_MESHEXPORTERRESULT

#include <vector>
#include "../VeritasEngineBase/Material.h"
#include "../VeritasEngineBase/ResourceId.h"
#include "MeshExporterVertex.h"
#include "../VeritasEngine/StringHash.h"
#include "../VeritasEngine/MatrixStack.h"

namespace VeritasACP
{
	struct MeshExporterSkeletonJoint
	{
		std::string Name;
		int ParentIndex;
		VeritasEngine::Matrix4x4 InverseBindPose;
	};

	struct MeshExporterSkeleton
	{
		std::vector<MeshExporterSkeletonJoint> Joints{};
		std::unordered_map<std::string, int> JointIndexMap{};
	};

	struct MeshExporterSubset
	{
		unsigned int m_vertexBaseIndex;
		unsigned int m_vertexCount;
		unsigned int m_indexBaseIndex;
		unsigned int m_indexCount;
		VeritasEngine::ResourceId m_material;
	};

	struct MeshExporterNode
	{
		VeritasEngine::Matrix4x4 m_transform{};
		std::vector<unsigned int> meshIndicies{};
		std::vector<MeshExporterNode> m_children{};
		int m_jointIndex{ -1 };
	};

	template <typename T>
	struct Channel
	{
		T m_data;
		float m_time;
	};

	struct AnimationBoneInformation
	{
		std::string m_jointName;
		int m_jointIndex;
		std::vector<Channel<VeritasEngine::Float3>> ScaleChannel;
		std::vector<Channel<VeritasEngine::Float3>> TranslationChannel;
		std::vector<Channel<VeritasEngine::Quaternion>> RotationChannel;
	};

	struct AnimationClipExporterResult
	{
		VeritasEngine::StringHash m_hashedName;
		float m_duration;
		std::vector<AnimationBoneInformation> m_poses;
		std::string m_name;
	};	

	struct MeshExporterResult
	{
		std::vector<MeshExporterVertex> m_verticies{};
		std::vector<unsigned int> m_indicies{};
		std::vector<MeshExporterSubset> m_subsets{};
		std::unordered_map<std::string, AnimationClipExporterResult> m_animations{};
		VeritasEngine::Matrix4x4 m_globalInverseTransform{};
		MeshExporterNode m_root{};
		MeshExporterSkeleton m_skeleton{};

	};
}

#endif
