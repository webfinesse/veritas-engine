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
		size_t ParentIndex;
		VeritasEngine::Matrix4x4 InverseBindPose;
	};

	struct MeshExporterSkeleton
	{
		MeshExporterSkeleton()
			: Joints{}, JointIndexMap{}
		{
			
		}

		std::vector<MeshExporterSkeletonJoint> Joints;
		std::unordered_map<std::string, size_t> JointIndexMap;
	};

	struct MeshExporterSubset
	{
		MeshExporterSubset()
			: m_vertices{}, m_faces {}, m_material{}
		{

		}

		std::vector<MeshExporterVertex> m_vertices;
		std::vector<unsigned int> m_faces;
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

	struct AnimationBoneInformation
	{
		std::string m_jointName;
		size_t m_jointIndex;
		std::vector<SqtExporterResult> m_keyframes;
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
		MeshExporterResult()
			: m_subsets{}, m_animations{}, m_root{}, m_skeleton{}
		{

		}

		std::vector<MeshExporterSubset> m_subsets;
		std::unordered_map<std::string, AnimationClipExporterResult> m_animations;
		MeshExporterNode m_root;
		MeshExporterSkeleton m_skeleton;
	};
}

#endif
