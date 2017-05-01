#ifndef H_SCENE
#define H_SCENE

#include <memory>
#include "SmallObject.h"

#include "IScene.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class GamePropertyManager;
	class FrameDescription;

	class Scene: public SmallObject<>, public IScene
	{
	public:
		Scene(std::shared_ptr<GamePropertyManager> gamePropertyManager);
		~Scene() noexcept override;

		Scene(Scene&& other) noexcept;
		Scene& operator=(Scene&& other) noexcept;

		void OnUpdate(const float deltaTime) override;
		void OnRender(FrameDescription& renderer) override;

		void Add(const GameObjectHandle handle) override;
		void AddChild(const GameObjectHandle parentHandle, const GameObjectHandle child) override;


	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)

#endif