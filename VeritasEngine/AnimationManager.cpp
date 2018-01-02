#include <vector>
#include <algorithm>

#include "AnimationManager.h"
#include "AnimationState.h"
#include "GamePropertyManager.h"
#include "../VeritasEngineBase/Animation.h"
#include "GameObjectPropertyKeys.h"
#include "MatrixStack.h"
#include "MeshNode.h"
#include "IJobManager.h"
#include "ResourceData.h"

struct VeritasEngine::AnimationManager::Impl
{
	Impl(std::shared_ptr<GamePropertyManager>&& gamePropertyManager, std::shared_ptr<IJobManager>&& jobManager)
		: m_gamePropertyManager{ std::move(gamePropertyManager) }, m_jobManager { std::move(jobManager) }
	{
		
	}

	void Init(std::shared_ptr<IResourceManager>&& resourceManager)
	{
		m_resourceManager = std::move(resourceManager);
	}

	Job* CalculatePoses(TimeDuration update)
	{
		const auto jobCallback = [&, update](Job* rootJob)
		{
			for (auto& anim : m_states)
			{
				const auto animPtr = &anim;
				const auto innerJob = [&, update, animPtr](Job* innerJobPtr)
				{
					const auto meshHandle = GetMesh(animPtr->Handle);

					m_resourceManager->GetResource(meshHandle, [&](const ResourceData& meshData)
					{
						const auto& meshInstance = meshData.GetData<MeshInstance>();
						auto animation = FindAnimation(meshInstance, animPtr->AnimationName);

						animPtr->Clock.SetDuration(TimeDuration{ animation->Duration });

						animPtr->Clock.Update(update);

						const auto currentTime = animPtr->Clock.GetCurrentTimeDuration().count();

						for (const auto& boneInfo : animation->BoneInfo)
						{
							const auto scale = GetChannel(boneInfo.ScaleChannel, currentTime);
							const auto rotation = GetChannel(boneInfo.RotationChannel, currentTime);
							const auto translation = GetChannel(boneInfo.TranslationChannel, currentTime);

							animPtr->LocalPoses[boneInfo.BoneIndex] = MathHelpers::CalculateSQT(scale, rotation, translation);
						}

						m_resourceManager->GetResource(meshInstance.GetSkeleton(), [&](const ResourceData& skeletonData)
						{
							const auto& skeleton = skeletonData.GetData<Skeleton>();
							MatrixStack matrixStack;
							CalculateGlobalPoses(meshInstance, skeleton, matrixStack, meshInstance.GetRootNode(), animPtr);
						});						
					});					
				};

				const auto calcJob = m_jobManager->CreateJobAsChild(rootJob, innerJob);
				m_jobManager->Run(calcJob);
			}
		};

		const auto job = m_jobManager->CreateJob(jobCallback);	

		m_jobManager->Run(job);

		return job;
	}

	ResourceHandle GetMesh(GameObjectHandle handle) const
	{
		if (m_meshProperty == nullptr)
		{
			m_meshProperty = m_gamePropertyManager->GetProperty<ResourceHandle>(GameObjectPropertyKeys::ResourcedMesh);
		}

		return *m_meshProperty->GetProperty(handle);
	}

	template <typename T>
	static T GetChannel(const std::vector<Channel<T>>& vector, float currentTime)
	{
		auto& keyFrame = std::lower_bound(vector.cbegin(), vector.cend(), currentTime, [](const Channel<T>& left, float valueToFind) { return left.Time < valueToFind; });

		if(keyFrame == vector.cend())
		{
			keyFrame = vector.cbegin();
		}

		auto previousKeyFrame = keyFrame;

		auto interpolationFactor = keyFrame->Time == 0 ? currentTime : (currentTime / keyFrame->Time);
		if (keyFrame != vector.cbegin())
		{
			previousKeyFrame = std::prev(previousKeyFrame);
			interpolationFactor = ((currentTime - previousKeyFrame->Time) / (keyFrame->Time - previousKeyFrame->Time));
		}

		return MathHelpers::Interpolate(previousKeyFrame->Data, keyFrame->Data, interpolationFactor);
	}

	static void CalculateGlobalPoses(const MeshInstance& meshInstance, const Skeleton& skeleton, MatrixStack& matrixStack, const MeshNode& currentNode, AnimationState* anim)
	{
		const auto jointIndex = currentNode.GetJointIndex();
		if (jointIndex >= 0)
		{
			matrixStack.Push(anim->LocalPoses[jointIndex]);
			anim->GlobalPoses[jointIndex] = meshInstance.GetGlobalInverseTransform() * matrixStack.Peek() * skeleton.Joints[jointIndex].InverseBindPose;			
		}
		else
		{
			matrixStack.Push(currentNode.GetTransform());
		}

		for (const auto& child : currentNode.GetChildren())
		{
			CalculateGlobalPoses(meshInstance, skeleton, matrixStack, child, anim);
		}

		matrixStack.Pop();
	}

	static Animation const* FindAnimation(const MeshInstance& meshInstance, StringHash animationName)
	{
		const auto& animations = meshInstance.GetAnimations();

		const auto& animation = std::find_if(animations.cbegin(), animations.cend(), [animationName](const auto& item) { return item.HashedName == animationName; });

		if(animation != animations.cend())
		{
			return &(*animation);
		}

		return nullptr;
	}

	mutable GameObjectProperty<ResourceHandle>* m_meshProperty{ nullptr };
	std::shared_ptr<GamePropertyManager> m_gamePropertyManager{ nullptr };
	std::shared_ptr<IJobManager> m_jobManager{ nullptr };
	std::shared_ptr<IResourceManager> m_resourceManager{ nullptr };
	std::vector<AnimationState> m_states{};
};

VeritasEngine::AnimationManager::AnimationManager(std::shared_ptr<GamePropertyManager> gamePropertyManager, std::shared_ptr<IJobManager> jobManager)
	: m_impl{ std::make_unique<Impl>(std::move(gamePropertyManager), std::move(jobManager)) }
{
	
}

void VeritasEngine::AnimationManager::Init(std::shared_ptr<IResourceManager> resourceManager)
{
	m_impl->Init(std::move(resourceManager));
}

void VeritasEngine::AnimationManager::AddAnimaton(GameObjectHandle handle, StringHash animationName, bool isLooped, float timeScale)
{
	m_impl->m_states.emplace_back(handle, animationName, TimeDuration{ 0 }, TimeDuration{ 0 }, isLooped, timeScale);
}

VeritasEngine::Job* VeritasEngine::AnimationManager::CalculatePoses(TimeDuration update)
{
	return m_impl->CalculatePoses(update);
}

const VeritasEngine::Matrix4x4* VeritasEngine::AnimationManager::GetGlobalPoses(GameObjectHandle handle)
{
	const auto& iter = std::find_if(m_impl->m_states.cbegin(), m_impl->m_states.cend(), [handle](AnimationState s) { return s.Handle == handle; });

	if(iter != m_impl->m_states.cend())
	{
		return iter->GlobalPoses;
	}

	return nullptr;
}
