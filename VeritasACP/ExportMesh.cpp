#include <vector>
#include <algorithm>
#include <string>

#include "ExportMesh.h"
#include "ExportMaterial.h"
#include "../VeritasEngine/Vertex.h"
#include "MeshExporterResult.h"


#define ASSIMP_DLL

#include "../Includes/assimp/include/assimp/Importer.hpp"
#include "../Includes/assimp/include/assimp/scene.h"
#include "../Includes/assimp/include/assimp/postprocess.h"

#include <boost/filesystem.hpp>

struct VeritasACP::ExportMesh::Impl
{
	Impl()
		:  m_processedMaterials{}
	{

	}

	MeshExporterSubset& AddSubset(std::shared_ptr<MeshExporterResult> meshInfo)
	{
		meshInfo->m_subsets.emplace_back();

		return meshInfo->m_subsets.back();
	}

	void ProcessVertex(aiMesh* mesh, MeshExporterSubset& meshInfo)
	{
		meshInfo.m_vertices.reserve(mesh->mNumVertices);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			auto& vertex = mesh->mVertices[i];

			meshInfo.m_vertices.emplace_back();
			auto& position = meshInfo.m_vertices.back().Position;
			position.x = vertex.x;
			position.y = vertex.y;
			position.z = vertex.z;
		}
	}

	void ProcessNormals(aiMesh* mesh, MeshExporterSubset& meshInfo)
	{
		if (mesh->HasNormals())
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				auto& vertex = meshInfo.m_vertices[i].Normal;
				vertex.x = mesh->mNormals[i].x;
				vertex.y = mesh->mNormals[i].y;
				vertex.z = mesh->mNormals[i].z;
			}
		}

		if (mesh->HasTangentsAndBitangents())
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				auto& tangent = meshInfo.m_vertices[i].Tangent;
				tangent.x = mesh->mTangents[i].x;
				tangent.y = mesh->mTangents[i].y;
				tangent.z = mesh->mTangents[i].z;

				auto& binormal = meshInfo.m_vertices[i].Binormal;
				binormal.x = mesh->mBitangents[i].x;
				binormal.y = mesh->mBitangents[i].y;
				binormal.z = mesh->mBitangents[i].z;
			}
		}
	}

	void ProcessUVCoords(aiMesh* mesh, MeshExporterSubset& meshInfo)
	{
		if (mesh->HasTextureCoords(0))
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				auto& vertex = meshInfo.m_vertices[i].TextureCoordinates;
				auto& coords = mesh->mTextureCoords[0][i];

				vertex.x = coords.x;
				vertex.y = coords.y;
			}
		}
	}

	void ProcessMaterial(const aiScene* scene, aiMesh* mesh, std::shared_ptr<MeshExporterResult> meshInfo)
	{
		auto item = m_processedMaterials.find(mesh->mMaterialIndex);

		if (item == m_processedMaterials.end())
		{
			ExportMaterial materialExporter;

			meshInfo->m_subsets.back().m_material = materialExporter.Export(m_basePath, scene, mesh);

			m_processedMaterials.emplace(mesh->mMaterialIndex, meshInfo->m_subsets.back().m_material);
		}
		else
		{
			meshInfo->m_subsets.back().m_material = item->second;
		}
	}

	void ProcessFaces(aiMesh* mesh, MeshExporterSubset& meshInfo)
	{
		if (mesh->HasFaces())
		{
			auto& facesCollection = meshInfo.m_faces;
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace& face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					facesCollection.emplace_back(face.mIndices[j]);
				}
			}
		}
	}

	VeritasEngine::Matrix4x4 ConvertTransform(const aiMatrix4x4& assImpMatrix)
	{
		assert(sizeof(VeritasEngine::Matrix4x4) == sizeof(aiMatrix4x4));

		VeritasEngine::Matrix4x4 result{};

		memcpy(&result, &assImpMatrix, sizeof(VeritasEngine::Matrix4x4));

		return result;
	}

	void ProcessNode(const aiNode* node, MeshExporterNode& meshNode)
	{
		meshNode.m_transform = ConvertTransform(node->mTransformation);
		
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			meshNode.meshIndicies.emplace_back(node->mMeshes[i]);
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			meshNode.m_children.emplace_back();

			auto& currentChild = meshNode.m_children.back();

			ProcessNode(node->mChildren[i], currentChild);
		}
	}


	fs::path m_basePath;
	std::unordered_map<unsigned int, VeritasEngine::ResourceId> m_processedMaterials;
};

VeritasACP::ExportMesh::ExportMesh()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasACP::ExportMesh::~ExportMesh() = default;

std::shared_ptr<VeritasACP::MeshExporterResult> VeritasACP::ExportMesh::Export(fs::path& fileName)
{
	Assimp::Importer importer;

	auto* scene = importer.ReadFile(fileName.generic_string().c_str(), aiProcess_ConvertToLeftHanded | aiProcess_TransformUVCoords | aiProcessPreset_TargetRealtime_Fast);
	
	if (scene == nullptr)
	{
		auto error = importer.GetErrorString();
		std::cout << error << std::endl;
	}
	else
	{
		m_impl->m_basePath = fileName.parent_path();

		if (scene->HasMeshes())
		{
			auto result = std::make_shared<MeshExporterResult>();

			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				auto& currentSubset = m_impl->AddSubset(result);

				auto* mesh = scene->mMeshes[i];

				m_impl->ProcessVertex(mesh, currentSubset);
				m_impl->ProcessNormals(mesh, currentSubset);
				m_impl->ProcessUVCoords(mesh, currentSubset);

				if (scene->HasMaterials())
				{
					m_impl->ProcessMaterial(scene, mesh, result);
				}

				m_impl->ProcessFaces(mesh, currentSubset);
			}

			m_impl->ProcessNode(scene->mRootNode, result->m_root);

			if (scene->mNumAnimations > 0)
			{

			}

			return result;
		}
	}
	
	return nullptr;
}