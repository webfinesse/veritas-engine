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
	struct SerializedMeshSubset
	{
		unsigned int m_vertexBaseIndex;
		unsigned int m_vertexCount;
		unsigned int m_indexBaseIndex;
		unsigned int m_indexCount;
		ResourceId m_materialId;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(m_vertexBaseIndex, m_vertexCount, m_indexBaseIndex, m_indexCount, m_materialId);
		}
	};

	struct SerializedMeshNode
	{
		VeritasEngine::Matrix4x4 m_transform{};
		std::vector<unsigned int> meshIndicies{};
		std::vector<SerializedMeshNode> m_children{};

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

		SerializedMeshNode m_root;
		std::vector<SerializedMeshSubset> m_subsets;
		std::vector<VertexType> m_verticies;
		std::vector<unsigned int> m_indicies;
		std::vector<Animation> m_animations;

		Matrix4x4 m_globalInverseTransform{};
		ResourceId m_skeletonId;
		VertexTypeHandle m_vertexType = VertexType::Type;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(m_root, m_subsets, m_verticies, m_indicies, m_animations, m_skeletonId, m_vertexType);
		}
	};

	using MeshInfo = MeshInfoGeneric<Vertex>;
	using SkinnedMeshInfo = MeshInfoGeneric<SkinnedVertex>;
}

#endif
