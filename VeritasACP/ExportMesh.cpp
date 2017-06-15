#include <vector>
#include <algorithm>
#include <string>

#include "ExportMesh.h"
#include "ExportMaterial.h"
#include "../VeritasEngine/Vertex.h"
#include "../VeritasEngine/StringHash.h"
#include "MeshExporterResult.h"


//#define ASSIMP_DLL

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
		if (scene->HasMaterials())
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
		static_assert(sizeof(VeritasEngine::Matrix4x4) == sizeof(aiMatrix4x4), "Matrix sizes do not match");

		VeritasEngine::Matrix4x4 result{};

		memcpy(&result, &assImpMatrix, sizeof(VeritasEngine::Matrix4x4));

		return result;
	}

	VeritasEngine::Float3 ConvertVec3(const aiVector3D& assImpVec)
	{
		static_assert(sizeof(VeritasEngine::Float3) == sizeof(aiVector3D), "Vector sizes do not match");

		VeritasEngine::Float3 result{};

		result.x = assImpVec.x;
		result.y = assImpVec.y;
		result.z = assImpVec.z;

		return result;
	}

	VeritasEngine::Quaternion ConvertQuaternion(const aiQuaternion& assImpQuaternion)
	{
		static_assert(sizeof(VeritasEngine::Quaternion) == sizeof(aiQuaternion), "Quaternion sizes do not match");

		VeritasEngine::Quaternion result{};

		result.w = assImpQuaternion.w;
		result.x = assImpQuaternion.x;
		result.y = assImpQuaternion.y;
		result.z = assImpQuaternion.z;

		return result;
	}

	void ProcessNode(const aiNode* node, MeshExporterNode& meshNode, MeshExporterResult& result, size_t currentJointIndex)
	{
		meshNode.m_transform = ConvertTransform(node->mTransformation);
		
		if(node->mName.length > 0)
		{
			auto nodeName = std::string(node->mName.C_Str());
			currentJointIndex = ProcessSkeletonBoneName(nodeName, result, currentJointIndex);
		}
		
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			meshNode.meshIndicies.emplace_back(node->mMeshes[i]);
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			meshNode.m_children.emplace_back();
			MeshExporterNode& currentChild = meshNode.m_children.back();

			ProcessNode(node->mChildren[i], currentChild, result, currentJointIndex);
		}
	}

	size_t static ProcessSkeletonBoneName(std::string& nodeName, MeshExporterResult& result, size_t currentJointIndex)
	{
		auto matchingJoint = result.m_skeleton.JointIndexMap.find(nodeName);

		if(matchingJoint != result.m_skeleton.JointIndexMap.end())
		{
			result.m_skeleton.Joints[matchingJoint->second].ParentIndex = currentJointIndex;
			return matchingJoint->second;
		}

		return currentJointIndex;
	}

	void ProcessBones(aiMesh* mesh, MeshExporterResult& meshResult, MeshExporterSubset& meshInfo)
	{
		if(mesh->HasBones())
		{
			for(unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
			{
				auto& currentBone = mesh->mBones[boneIndex];				
				auto existingJoint = meshResult.m_skeleton.JointIndexMap.find(currentBone->mName.C_Str());

				if (existingJoint == meshResult.m_skeleton.JointIndexMap.end())
				{
					MeshExporterSkeletonJoint joint;
					joint.Name = std::string(currentBone->mName.C_Str());
					joint.InverseBindPose = ConvertTransform(currentBone->mOffsetMatrix);
					joint.ParentIndex = -1;

					for (unsigned int weightIndex = 0; weightIndex < currentBone->mNumWeights; weightIndex++)
					{
						auto& currentWeight = currentBone->mWeights[weightIndex];
						auto& vertex = meshInfo.m_vertices[currentWeight.mVertexId];
						vertex.JointWeights.emplace_back(currentWeight.mWeight);
						vertex.JointIndicies.emplace_back(boneIndex);
					}

					meshResult.m_skeleton.Joints.emplace_back(joint);
					meshResult.m_skeleton.JointIndexMap[joint.Name] = meshResult.m_skeleton.Joints.size() - 1;
				}
			}
		}
	}

	void ProcessAnimations(const aiScene* scene, MeshExporterResult& result)
	{
		if(scene->HasAnimations())
		{
			for(unsigned int animationIndex = 0; animationIndex < scene->mNumAnimations; animationIndex++)
			{
				auto animation = scene->mAnimations[animationIndex];

				AnimationClipExporterResult clip;
				clip.m_duration = animation->mTicksPerSecond == 0 ? static_cast<float>(animation->mDuration) : static_cast<float>(animation->mTicksPerSecond / animation->mDuration);
				clip.m_hashedName = VeritasEngine::Hash(animation->mName.C_Str());
				clip.m_name = std::string(animation->mName.C_Str());

				for(unsigned int channelIndex = 0; channelIndex < animation->mNumChannels; channelIndex++)
				{
					auto channel = animation->mChannels[channelIndex];

					clip.m_poses.emplace_back();
					auto& currentPose = clip.m_poses.back();

					currentPose.m_jointName = std::string(channel->mNodeName.C_Str());
					currentPose.m_jointIndex = result.m_skeleton.JointIndexMap[currentPose.m_jointName];

					for (unsigned int poseIndex = 0; poseIndex < channel->mNumScalingKeys; poseIndex++)
					{
						currentPose.m_keyframes.emplace_back();
						auto& keyFrame = currentPose.m_keyframes.back();
						
						keyFrame.m_timeSample = static_cast<float>(channel->mScalingKeys[poseIndex].mTime);
						keyFrame.m_scale = ConvertVec3(channel->mScalingKeys[poseIndex].mValue);
						keyFrame.m_rotation = ConvertQuaternion(channel->mRotationKeys[poseIndex].mValue);
						keyFrame.m_translation = ConvertVec3(channel->mPositionKeys[poseIndex].mValue);
					}
				}

				result.m_animations.emplace(make_pair(clip.m_name, clip));
				
			}
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
				m_impl->ProcessMaterial(scene, mesh, result);
				m_impl->ProcessFaces(mesh, currentSubset);
				m_impl->ProcessBones(mesh, *result, currentSubset);
			}

			m_impl->ProcessNode(scene->mRootNode, result->m_root, *result, -1);
			m_impl->ProcessAnimations(scene, *result);

			return result;
		}
	}
	
	return nullptr;
}