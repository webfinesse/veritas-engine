#ifndef H_MESHEXPORTERRESULT
#define H_MESHEXPORTERRESULT

#include <memory>
#include <vector>
#include "../VeritasEngine/Vertex.h"
#include "../VeritasEngineBase/Material.h"
#include "../VeritasEngineBase/ResourceId.h"

namespace VeritasACP
{
	struct MeshExporterSubset
	{
		MeshExporterSubset()
			: m_vertices{}, m_faces {}, m_material{}
		{

		}

		std::vector<VeritasEngine::Vertex> m_vertices;
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
	

	struct MeshExporterResult
	{
		MeshExporterResult()
			: m_subsets{}, m_root{}
		{

		}

		std::vector<MeshExporterSubset> m_subsets;
		MeshExporterNode m_root;
	};
}

#endif
