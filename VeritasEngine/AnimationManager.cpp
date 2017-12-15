#include <vector>
#include <algorithm>

#include "AnimationManager.h"
#include "AnimationState.h"
#include "GamePropertyManager.h"
#include "../VeritasEngineBase/Animation.h"
#include "GameObjectPropertyKeys.h"
#include "../VeritasEngineBase/ResourceHandle.h"
#include "MatrixStack.h"
#include "MeshNode.h"
#include "IJobManager.h"

struct GetAnimationResult
{
	const VeritasEngine::Animation* Animation{};
	const VeritasEngine::Skeleton* Skeleton{};
	const VeritasEngine::MeshInstance* Mesh{};
};


struct VeritasEngine::AnimationManager::Impl
{
	Impl(std::shared_ptr<GamePropertyManager>&& gamePropertyManager, std::shared_ptr<IJobManager>&& jobManager)
		: m_gamePropertyManager{ std::move(gamePropertyManager) }, m_jobManager { std::move(jobManager) }
	{
		
	}

	Job* CalculatePoses(TimeDuration update)
	{
		const auto jobCallback = [&, update](Job* job, const void* data)
		{
			for (auto& anim : m_states)
			{
				const auto innerJob = [&, update](Job* job, const void* data)
				{
					const auto animationResult = GetAnimation(anim.Handle, anim.AnimationName);

					if (animationResult.Animation != nullptr)
					{
						anim.Clock.Update(update);

						const auto currentTime = anim.Clock.GetCurrentTime().count();

						for (const auto& boneInfo : animationResult.Animation->BoneInfo)
						{
							const auto scale = GetChannel(boneInfo.ScaleChannel, currentTime);
							const auto rotation = GetChannel(boneInfo.RotationChannel, currentTime);
							const auto translation = GetChannel(boneInfo.TranslationChannel, currentTime);

							anim.LocalPoses[boneInfo.BoneIndex] = MathHelpers::CalculateSQT(scale, rotation, translation);
						}

						MatrixStack matrixStack;
						CalculateGlobalPoses(animationResult, matrixStack, animationResult.Mesh->GetRootNode(), anim);
					}
				};

				const auto calcJob = m_jobManager->CreateJobAsChild(job, innerJob);
				m_jobManager->Run(calcJob);
			}
		};

		const auto job = m_jobManager->CreateJob(jobCallback);	

		m_jobManager->Run(job);

		return job;
	}

	ResourceHandle* GetMesh(GameObjectHandle handle) const
	{
		if (m_meshProperty == nullptr)
		{
			m_meshProperty = m_gamePropertyManager->GetProperty<ResourceHandle*>(GameObjectPropertyKeys::ResourcedMesh);
		}

		return m_meshProperty->GetProperty(handle);
	}

	GetAnimationResult GetAnimation(GameObjectHandle handle, StringHash animationName) const
	{
		const auto mesh = GetMesh(handle);
		const auto& meshInstance = mesh->GetData<MeshInstance>();

		const auto& animations = meshInstance.GetAnimations();

		const auto& animation = std::find_if(animations.cbegin(), animations.cend(), [animationName](const auto& item) { return item.HashedName == animationName; });

		GetAnimationResult result;

		if (animation != animations.cend())
		{
			result.Animation = &*animation;
		}

		result.Mesh = &meshInstance;
		result.Skeleton = &meshInstance.GetSkeleton()->GetData<Skeleton>();
		
		return result;
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

	static void CalculateGlobalPoses(const GetAnimationResult& animationResult, MatrixStack& matrixStack, const MeshNode& currentNode, AnimationState& anim)
	{
		const auto jointIndex = currentNode.GetJointIndex();
		if (jointIndex >= 0)
		{
			matrixStack.Push(anim.LocalPoses[jointIndex]);

			anim.GlobalPoses[jointIndex] = animationResult.Mesh->GetGlobalInverseTransform() * matrixStack.Peek() * animationResult.Skeleton->Joints[jointIndex].InverseBindPose;
		}
		else
		{
			matrixStack.Push(currentNode.GetTransform());
		}

		for (const auto& child : currentNode.GetChildren())
		{
			CalculateGlobalPoses(animationResult, matrixStack, child, anim);
		}

		matrixStack.Pop();
	}

	mutable GameObjectProperty<ResourceHandle*>* m_meshProperty{ nullptr };
	std::shared_ptr<GamePropertyManager> m_gamePropertyManager{ nullptr };
	std::shared_ptr<IJobManager> m_jobManager{ nullptr };
	std::vector<AnimationState> m_states{};
};

VeritasEngine::AnimationManager::AnimationManager(std::shared_ptr<GamePropertyManager> gamePropertyManager, std::shared_ptr<IJobManager> jobManager)
	: m_impl{ std::make_unique<Impl>(std::move(gamePropertyManager), std::move(jobManager)) }
{
	
}

void VeritasEngine::AnimationManager::AddAnimaton(GameObjectHandle handle, StringHash animationName, bool isLooped, float timeScale)
{
	const auto animationResult = m_impl->GetAnimation(handle, animationName);

	if(animationResult.Animation != nullptr)
	{
		m_impl->m_states.emplace_back(handle, animationName, TimeDuration{ animationResult.Animation->Duration }, TimeDuration{ 0 }, isLooped, timeScale);
	}
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
