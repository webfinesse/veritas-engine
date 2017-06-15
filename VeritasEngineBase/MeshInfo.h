#ifndef H_ENGINEMESHINFO
#define H_ENGINEMESHINFO

#include <vector>
#include "../VeritasEngine/Vertex.h"
#include "../VeritasEngineBase/ResourceId.h"
#include "../VeritasEngineBase/Animation.h"

#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "../VeritasEngine/SkinnedVertex.h"
#include "../VeritasEngine/VertexTypeHandle.h"

namespace VeritasEngine
{
	template<typename VertexFormat>
	struct SerializedMeshSubsetGeneric
	{
		SerializedMeshSubsetGeneric()
			: m_verticies{}, m_faces {}, m_materialId{}
		{

		}

		std::vector<VertexFormat> m_verticies;
		std::vector<unsigned int> m_faces;
		ResourceId m_materialId;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(m_verticies, m_faces, m_materialId);
		}
	};

	struct SerializedMeshNode
	{
		SerializedMeshNode()
			: m_transform{}, meshIndicies{}, m_children{}
		{

		}

		VeritasEngine::Matrix4x4 m_transform;
		std::vector<unsigned int> meshIndicies;
		std::vector<SerializedMeshNode> m_children;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(m_transform, meshIndicies, m_children);
		}
	};

	template<typename VertexFormat>
	struct MeshInfoGeneric
	{
		using VertexType = VertexFormat;
		using SubsetType = SerializedMeshSubsetGeneric<VertexFormat>;

		SerializedMeshNode m_root;
		std::vector<SubsetType> m_subsets;
		std::vector<Animation> m_animations;
		ResourceId m_skeletonId;
		VertexTypeHandle m_vertexType = VertexType::Type;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(m_root, m_subsets, m_animations, m_skeletonId, m_vertexType);
		}
	};

	using MeshInfo = MeshInfoGeneric<Vertex>;
	using SkinnedMeshInfo = MeshInfoGeneric<SkinnedVertex>;
}

#endif
