#ifndef H_MESHEXPORTERRESULT
#define H_MESHEXPORTERRESULT

#include <vector>
#include "../VeritasEngineBase/Material.h"
#include "../VeritasEngineBase/ResourceId.h"
#include "MeshExporterVertex.h"
#include "../VeritasEngine/StringHash.h"

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
		MeshExporterSkeleton()
			: Joints{}, JointIndexMap{}
		{
			
		}

		std::vector<MeshExporterSkeletonJoint> Joints;
		std::unordered_map<std::string, unsigned int> JointIndexMap;
	};

	struct MeshExporterSubset
	{
		MeshExporterSubset()
			: m_vertices{}, m_faces {}, m_skeletons{}, m_material{}
		{

		}

		std::vector<MeshExporterVertex> m_vertices;
		std::vector<unsigned int> m_faces;
		std::vector<MeshExporterSkeleton> m_skeletons;
		VeritasEngine::ResourceId m_material;
	};

	struct MeshExporterNode
	{
		MeshExporterNode()
			: m_transform{}, meshIndicies{}, m_children{}
		{

		}

		VeritasEngine::Matrix4x4 m_transform;
		std::vector<unsigned int> meshIndicies;
		std::vector<MeshExporterNode> m_children;
	};

	struct SqtExporterResult
	{
		VeritasEngine::Float3 m_scale; // 0 - 96
		VeritasEngine::Quaternion m_rotation; // 96 - 224
		VeritasEngine::Float3 m_translation; // 224 - 320
		float m_timeSample; // 320 - 352
	};

	struct AnimationPoseExporterResult
	{
		std::vector<SqtExporterResult> m_jointPoses;
	};

	struct AnimationClipExporterResult
	{
		VeritasEngine::StringHash m_hashedName;
		float m_duration;
		std::vector<AnimationPoseExporterResult> m_poses;
	};
	

	struct MeshExporterResult
	{
		MeshExporterResult()
			: m_subsets{}, m_animations{}, m_root{}
		{

		}

		std::vector<MeshExporterSubset> m_subsets;
		std::unordered_map<std::string, AnimationClipExporterResult> m_animations;
		MeshExporterNode m_root;
	};
}

#endif
