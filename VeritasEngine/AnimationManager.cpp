#include "AnimationManager.h"
#include "AnimationState.h"
#include "GamePropertyManager.h"
#include "../VeritasEngineBase/Animation.h"
#include "GameObjectPropertyKeys.h"
#include "../VeritasEngineBase/ResourceHandle.h"

#include <vector>
#include <algorithm>


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

	mutable GameObjectProperty<ResourceHandle*>* m_meshProperty{ nullptr };
	std::shared_ptr<GamePropertyManager> m_gamePropertyManager;
	std::vector<AnimationState> m_states{};
};

VeritasEngine::AnimationManager::AnimationManager(std::shared_ptr<GamePropertyManager> gamePropertyManager)
	: m_impl{ std::make_unique<Impl>(gamePropertyManager) }
{
	
}

void VeritasEngine::AnimationManager::AddAnimaton(GameObjectHandle handle, StringHash animationName, bool isLooped)
{
	const auto mesh = m_impl->GetMesh(handle);
	const auto& meshInstance = mesh->GetData<MeshInstance>();

	const auto& animations = meshInstance.GetAnimations();

	const auto& result = std::find_if(animations.cbegin(), animations.cend(), [animationName](const auto& item) { return item.HashedName == animationName; });

	if(result != animations.cend())
	{
		m_impl->m_states.emplace_back(handle, animationName, TimeDuration{ result->Duration }, TimeDuration{ 0 }, isLooped);
	}
}
