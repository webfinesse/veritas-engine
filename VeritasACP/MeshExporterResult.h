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
		MeshExporterSkeleton()
			: Joints{}, JointIndexMap{}
		{
			
		}

		std::vector<MeshExporterSkeletonJoint> Joints;
		std::unordered_map<std::string, int> JointIndexMap;
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
		int m_jointIndex;
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
