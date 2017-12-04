#include <vector>
#include <algorithm>
#include <string>
#include <numeric>

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
#include "../VeritasEngine/Skeleton.h"
#include "../VeritasEngineBase/StringHelper.h"

struct VeritasACP::ExportMesh::Impl
{
	Impl()
		: m_processedMaterials{}
	{

	}

	static MeshExporterSubset& AddSubset(std::shared_ptr<MeshExporterResult> meshInfo)
	{
		meshInfo->m_subsets.emplace_back();

		return meshInfo->m_subsets.back();
	}

	static void ProcessVertex(aiMesh* mesh, MeshExporterResult& result, MeshExporterSubset& subset)
	{
		subset.m_vertexBaseIndex = result.m_verticies.size();
		subset.m_vertexCount = mesh->mNumVertices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			auto& vertex = mesh->mVertices[i];

			result.m_verticies.emplace_back();
			auto& position = result.m_verticies.back().Position;
			position.x = vertex.x;
			position.y = vertex.y;
			position.z = vertex.z;
		}
	}

	static void ProcessNormals(aiMesh* mesh, MeshExporterResult& result, MeshExporterSubset& subset)
	{
		if (mesh->HasNormals())
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				auto& vertex = result.m_verticies[subset.m_vertexBaseIndex + i].Normal;
				vertex.x = mesh->mNormals[i].x;
				vertex.y = mesh->mNormals[i].y;
				vertex.z = mesh->mNormals[i].z;
			}
		}

		if (mesh->HasTangentsAndBitangents())
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				auto& tangent = result.m_verticies[subset.m_vertexBaseIndex + i].Tangent;
				tangent.x = mesh->mTangents[i].x;
				tangent.y = mesh->mTangents[i].y;
				tangent.z = mesh->mTangents[i].z;

				auto& binormal = result.m_verticies[subset.m_vertexBaseIndex + i].Binormal;
				binormal.x = mesh->mBitangents[i].x;
				binormal.y = mesh->mBitangents[i].y;
				binormal.z = mesh->mBitangents[i].z;
			}
		}
	}

	static void ProcessUVCoords(aiMesh* mesh, MeshExporterResult& result, MeshExporterSubset& subset)
	{
		if (mesh->HasTextureCoords(0))
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				auto& vertex = result.m_verticies[subset.m_vertexBaseIndex + i].TextureCoordinates;
				auto& coords = mesh->mTextureCoords[0][i];

				vertex.x = coords.x;
				vertex.y = coords.y;
			}
		}
	}

	void ProcessMaterial(const aiScene* scene, aiMesh* mesh, MeshExporterResult& result, MeshExporterSubset& subset)
	{
		if (scene->HasMaterials())
		{
			const auto item = m_processedMaterials.find(mesh->mMaterialIndex);

			if (item == m_processedMaterials.end())
			{
				ExportMaterial materialExporter;

				const auto materialResult = materialExporter.Export(m_basePath, scene, mesh);
				result.m_subsets.back().m_material = materialResult.ResourceName;

				const auto heightRatio = materialResult.NewTextureHeight / float(materialResult.OriginalTextureHeight ? materialResult.OriginalTextureHeight : 1);
				const auto widthRatio = materialResult.NewTextureWidth / float(materialResult.OriginalTextureWidth ? materialResult.OriginalTextureWidth : 1);

				for (unsigned int i = 0; i < mesh->mNumVertices; i++)
				{
					result.m_verticies[subset.m_vertexBaseIndex + i].TextureCoordinates.x *= widthRatio;
					result.m_verticies[subset.m_vertexBaseIndex + i].TextureCoordinates.y *= heightRatio;
				}

				m_processedMaterials.emplace(mesh->mMaterialIndex, result.m_subsets.back().m_material);
			}
			else
			{
				result.m_subsets.back().m_material = item->second;
			}
		}
	}

	static void ProcessFaces(aiMesh* mesh, MeshExporterResult& result, MeshExporterSubset& subset)
	{
		if (mesh->HasFaces())
		{
			subset.m_indexBaseIndex = result.m_indicies.size();			

			auto& facesCollection = result.m_indicies;
			const auto& initialIndex = facesCollection.size();
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace& face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					facesCollection.emplace_back(face.mIndices[j]);
				}
			}

			subset.m_indexCount = facesCollection.size() - initialIndex;
		}
	}

	static VeritasEngine::Matrix4x4 ConvertTransform(const aiMatrix4x4& assImpMatrix)
	{
		static_assert(sizeof(VeritasEngine::Matrix4x4) == sizeof(aiMatrix4x4), "Matrix sizes do not match");

		// assimp is a row major library, however we are a column major engine, so we need to transpose the matrix

		VeritasEngine::Matrix4x4 result{};

		auto transposed = assImpMatrix;
		transposed.Transpose();

		memcpy(&result, &transposed, sizeof(VeritasEngine::Matrix4x4));

		return result;
	}

	static VeritasEngine::Float3 ConvertVec3(const aiVector3D& assImpVec)
	{
		static_assert(sizeof(VeritasEngine::Float3) == sizeof(aiVector3D), "Vector sizes do not match");

		VeritasEngine::Float3 result{};

		result.x = assImpVec.x;
		result.y = assImpVec.y;
		result.z = assImpVec.z;

		return result;
	}

	static VeritasEngine::Quaternion ConvertQuaternion(const aiQuaternion& assImpQuaternion)
	{
		static_assert(sizeof(VeritasEngine::Quaternion) == sizeof(aiQuaternion), "Quaternion sizes do not match");

		VeritasEngine::Quaternion result{};

		result.w = assImpQuaternion.w;
		result.x = assImpQuaternion.x;
		result.y = assImpQuaternion.y;
		result.z = assImpQuaternion.z;

		return result;
	}

	static void ProcessNode(const aiNode* node, MeshExporterNode& meshNode, MeshExporterResult& result, int currentJointIndex)
	{
		meshNode.m_transform = ConvertTransform(node->mTransformation);

		if (node->mName.length > 0)
		{
			auto nodeName = std::string(node->mName.C_Str());
			currentJointIndex = ProcessSkeletonBoneName(nodeName, result, currentJointIndex);
			meshNode.m_jointIndex = currentJointIndex;
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

	int static ProcessSkeletonBoneName(std::string& nodeName, MeshExporterResult& result, int currentJointIndex)
	{
		const auto matchingJoint = result.m_skeleton.JointIndexMap.find(nodeName);

		if (matchingJoint != result.m_skeleton.JointIndexMap.end())
		{
			result.m_skeleton.Joints[matchingJoint->second].ParentIndex = currentJointIndex;
			return matchingJoint->second;
		}

		return currentJointIndex;
	}

	static void ProcessBones(aiMesh* mesh, MeshExporterResult& meshResult, MeshExporterSubset& meshInfo)
	{
		if (mesh->HasBones())
		{
			if(mesh->mNumBones > MAX_JOINTS)
			{
				throw new std::runtime_error(VeritasEngine::FormatString("The skeleton has more then %d bones, reduce the number of bones in the skeleton", MAX_JOINTS));
			}

			for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
			{
				const auto currentBone = mesh->mBones[boneIndex];
				const auto existingJoint = meshResult.m_skeleton.JointIndexMap.find(currentBone->mName.C_Str());

				auto index = meshResult.m_skeleton.Joints.size();

				if (existingJoint == meshResult.m_skeleton.JointIndexMap.end())
				{
					MeshExporterSkeletonJoint joint;
					joint.Name = std::string(currentBone->mName.C_Str());
					joint.InverseBindPose = ConvertTransform(currentBone->mOffsetMatrix);
					joint.ParentIndex = -1;

					meshResult.m_skeleton.Joints.emplace_back(joint);
					meshResult.m_skeleton.JointIndexMap[joint.Name] = static_cast<int>(index);
				}
				else
				{
					index = existingJoint->second;
				}

				for (unsigned int weightIndex = 0; weightIndex < currentBone->mNumWeights; weightIndex++)
				{
					auto& currentWeight = currentBone->mWeights[weightIndex];
					auto& vertex = meshResult.m_verticies[meshInfo.m_vertexBaseIndex + currentWeight.mVertexId];

					const auto existingIndex = std::find_if(vertex.JointIndicies.cbegin(), vertex.JointIndicies.cend(), [index](std::byte jointIndex) { return std::size_t(jointIndex) == index; });
					if (existingIndex == vertex.JointIndicies.cend())
					{
						vertex.JointWeights.emplace_back(currentWeight.mWeight);
						vertex.JointIndicies.emplace_back(static_cast<std::byte>(index));
					}
					else
					{
						assert(vertex.JointWeights[existingIndex - vertex.JointIndicies.cbegin()] == currentWeight.mWeight);
					}
				}				
			}
		}
	}

	static void ProcessAnimations(const aiScene* scene, MeshExporterResult& result)
	{
		if (scene->HasAnimations())
		{
			for (unsigned int animationIndex = 0; animationIndex < scene->mNumAnimations; animationIndex++)
			{
				const auto animation = scene->mAnimations[animationIndex];

				AnimationClipExporterResult clip;
				const auto ticksPerSec = animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 1.0f;
				clip.m_duration = float(animation->mDuration / ticksPerSec);
				clip.m_hashedName = VeritasEngine::Hash(animation->mName.C_Str());
				clip.m_name = std::string(animation->mName.C_Str());

				for (unsigned int channelIndex = 0; channelIndex < animation->mNumChannels; channelIndex++)
				{
					const auto channel = animation->mChannels[channelIndex];

					const auto jointName = std::string(channel->mNodeName.C_Str());
					const auto& mapping  = result.m_skeleton.JointIndexMap.find(jointName);

					if(mapping != result.m_skeleton.JointIndexMap.end())
					{
						clip.m_poses.emplace_back();
						auto& currentPose = clip.m_poses.back();

						currentPose.m_jointName = std::move(jointName);
						currentPose.m_jointIndex = mapping->second;						

						for (unsigned int scaleIndex = 0; scaleIndex < channel->mNumScalingKeys; scaleIndex++)
						{
							currentPose.ScaleChannel.emplace_back();
							auto& pose = currentPose.ScaleChannel.back();
							pose.m_data = ConvertVec3(channel->mScalingKeys[scaleIndex].mValue);
							pose.m_time = channel->mScalingKeys[scaleIndex].mTime / ticksPerSec;
						}

						for (unsigned int translationIndex = 0; translationIndex < channel->mNumPositionKeys; translationIndex++)
						{
							currentPose.TranslationChannel.emplace_back();
							auto& pose = currentPose.TranslationChannel.back();
							pose.m_data = ConvertVec3(channel->mPositionKeys[translationIndex].mValue);
							pose.m_time = channel->mPositionKeys[translationIndex].mTime / ticksPerSec;
						}
						
						for (unsigned int rotationIndex = 0; rotationIndex < channel->mNumRotationKeys; rotationIndex++)
						{
							currentPose.RotationChannel.emplace_back();
							auto& pose = currentPose.RotationChannel.back();
							pose.m_data = ConvertQuaternion(channel->mRotationKeys[rotationIndex].mValue);
							pose.m_time = channel->mRotationKeys[rotationIndex].mTime / ticksPerSec;
						}
					}
				}

				std::sort(clip.m_poses.begin(), clip.m_poses.end(), [&result](auto left, auto right) { return result.m_skeleton.Joints[left.m_jointIndex].ParentIndex < result.m_skeleton.Joints[right.m_jointIndex].ParentIndex; });

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
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_CAMERAS | aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_TANGENTS_AND_BITANGENTS);
	importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);

	auto* scene = importer.ReadFile(fileName.generic_string().c_str(), aiProcess_ConvertToLeftHanded |
		aiProcess_TransformUVCoords |
		aiProcessPreset_TargetRealtime_Fast |
		aiProcess_LimitBoneWeights |
		aiProcess_RemoveComponent |
		aiProcess_RemoveRedundantMaterials);

	if (scene == nullptr)
	{
		const auto error = importer.GetErrorString();
		std::cout << error << std::endl;
	}
	else
	{
		m_impl->m_basePath = fileName.parent_path();

		if (scene->HasMeshes())
		{
			auto result = std::make_shared<MeshExporterResult>();

			auto verticiesLength = 0;
			auto facesLength = 0;
			for (auto i = 0; i < scene->mNumMeshes; i++)
			{
				verticiesLength += scene->mMeshes[i]->mNumVertices;
				facesLength += scene->mMeshes[i]->mNumFaces * 3;
			}

			result->m_subsets.reserve(scene->mNumMeshes);
			result->m_verticies.reserve(verticiesLength);
			result->m_indicies.reserve(facesLength);

			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				auto& currentSubset = m_impl->AddSubset(result);

				auto* mesh = scene->mMeshes[i];				

				m_impl->ProcessVertex(mesh, *result, currentSubset);
				m_impl->ProcessNormals(mesh, *result, currentSubset);
				m_impl->ProcessUVCoords(mesh, *result, currentSubset);
				m_impl->ProcessMaterial(scene, mesh, *result, currentSubset);
				m_impl->ProcessFaces(mesh, *result, currentSubset);
				m_impl->ProcessBones(mesh, *result, currentSubset);
			}

			m_impl->ProcessNode(scene->mRootNode, result->m_root, *result, -1);
			m_impl->ProcessAnimations(scene, *result);

			auto rootTransform = scene->mRootNode->mTransformation;
			rootTransform.Inverse();
			result->m_globalInverseTransform = m_impl->ConvertTransform(rootTransform);

			return result;
		}
	}

	return nullptr;
}