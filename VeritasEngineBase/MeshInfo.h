#ifndef H_ENGINEMESHINFO
#define H_ENGINEMESHINFO

#include <vector>
#include "../VeritasEngine/Vertex.h"
#include "../VeritasEngineBase/ResourceId.h"

#include "../Includes/cereal-1.1.2/include/cereal/types/vector.hpp"
#include "../Includes/cereal-1.1.2/include/cereal/types/string.hpp"

namespace VeritasEngine
{
	struct SerializedMeshSubset
	{
		SerializedMeshSubset()
			: m_verticies{}, m_faces {}, m_materialId{}
		{

		}

		std::vector<VeritasEngine::Vertex> m_verticies;
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

	struct MeshInfo
	{
		SerializedMeshNode m_root;
		std::vector<VeritasEngine::SerializedMeshSubset> m_subsets;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(m_root, m_subsets);
		}
	};
}

#endif
