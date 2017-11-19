#include "AnimationManager.h"
#include "AnimationState.h"
#include "GamePropertyManager.h"
#include "../VeritasEngineBase/Animation.h"
#include "GameObjectPropertyKeys.h"
#include "../VeritasEngineBase/ResourceHandle.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <algorithm>

struct GetAnimationResult
{
	const VeritasEngine::Animation* Animation{};
	const VeritasEngine::Skeleton* Skeleton{};
	const VeritasEngine::MeshInstance* Mesh{};
};


struct VeritasEngine::AnimationManager::Impl
{
	Impl(std::shared_ptr<GamePropertyManager> gamePropertyManager)
		: m_gamePropertyManager{ gamePropertyManager }
	{
		
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

	mutable GameObjectProperty<ResourceHandle*>* m_meshProperty{ nullptr };
	std::shared_ptr<GamePropertyManager> m_gamePropertyManager;
	std::vector<AnimationState> m_states{};
};

VeritasEngine::AnimationManager::AnimationManager(std::shared_ptr<GamePropertyManager> gamePropertyManager)
	: m_impl{ std::make_unique<Impl>(gamePropertyManager) }
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

void VeritasEngine::AnimationManager::CalculateSkinningPalettes(TimeDuration update)
{
	for(auto& anim : m_impl->m_states)
	{
		const auto animationResult = m_impl->GetAnimation(anim.Handle, anim.AnimationName);

		if(animationResult.Animation != nullptr)
		{
			anim.Clock.Update(update);

			//const auto currentTime = 0;
			//const auto currentTime = TimeDuration(0.5f).count();
			const auto currentTime = anim.Clock.GetCurrentTime().count();

			for(const auto& boneInfo : animationResult.Animation->BoneInfo)
			{
				const auto& keyFrame = std::lower_bound(boneInfo.Keyframes.cbegin(), boneInfo.Keyframes.cend(), currentTime, [](const Keyframe& left, float valueToFind) { return left.TimeSample < valueToFind; });
				auto previousKeyFrame = keyFrame;

				if(keyFrame->TimeSample != currentTime && keyFrame != boneInfo.Keyframes.cbegin())
				{
					previousKeyFrame = std::prev(previousKeyFrame);
				}

				const auto interpolationFactor = previousKeyFrame == boneInfo.Keyframes.cbegin() ? 0 : ((currentTime - previousKeyFrame->TimeSample) / (keyFrame->TimeSample - previousKeyFrame->TimeSample));

				const auto scale = MathHelpers::Interpolate(previousKeyFrame->Scale, keyFrame->Scale, interpolationFactor);
				const auto rotation = MathHelpers::Interpolate(previousKeyFrame->Rotation, keyFrame->Rotation, interpolationFactor);
				const auto translation = MathHelpers::Interpolate(previousKeyFrame->Translation, keyFrame->Translation, interpolationFactor);

				const auto parentIndex = animationResult.Skeleton->Joints[boneInfo.BoneIndex].ParentIndex;
				const auto& inverseBindPose = animationResult.Skeleton->Joints[boneInfo.BoneIndex].InverseBindPose;

				const auto result = MathHelpers::CalculateSQT(scale, rotation, translation);

				if(parentIndex == -1)
				{
					anim.SkinningPalette[boneInfo.BoneIndex] = inverseBindPose * result;
					//anim.SkinningPalette[boneInfo.BoneIndex] = animationResult.Mesh->GetGlobalInverseTransform() * result * inverseBindPose;
				}
				else
				{
					anim.SkinningPalette[boneInfo.BoneIndex] = inverseBindPose * result * anim.SkinningPalette[parentIndex];
					//anim.SkinningPalette[boneInfo.BoneIndex] = animationResult.Mesh->GetGlobalInverseTransform() * anim.SkinningPalette[parentIndex] * result * inverseBindPose;
				}
			}
		}
	}
}

const VeritasEngine::Matrix4x4* VeritasEngine::AnimationManager::GetSkinningPalette(GameObjectHandle handle)
{
	const auto& iter = std::find_if(m_impl->m_states.cbegin(), m_impl->m_states.cend(), [handle](AnimationState s) { return s.Handle == handle; });

	if(iter != m_impl->m_states.cend())
	{
		return iter->SkinningPalette;
	}

	return nullptr;
}
