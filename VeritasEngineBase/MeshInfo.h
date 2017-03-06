#ifndef H_ENGINEMESHINFO
#define H_ENGINEMESHINFO

#include <vector>
#include "../VeritasEngine/Vertex.h"
#include "../VeritasEngineBase/ResourceId.h"

#include "../Includes/cereal-1.1.2/include/cereal/types/vector.hpp"
#include "../Includes/cereal-1.1.2/include/cereal/types/string.hpp"
#include "../VeritasEngine/SkinnedVertex.h"

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
		ResourceId m_skeletonId;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(m_root, m_subsets, m_skeletonId);
		}
	};

	using MeshInfo = MeshInfoGeneric<Vertex>;
	using SkinnedMeshInfo = MeshInfoGeneric<SkinnedVertex>;
}

#endif
