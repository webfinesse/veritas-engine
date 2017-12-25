#pragma once

#include "DynamicLibraryHelper.h"
#include "../VeritasEngineBase/GameObjectHandle.h"

namespace VeritasEngine
{
	class IRenderer;
	class IMeshShader;
	class FrameDescription;
	class IResourceManager;

	class EXPORT IScene
	{
	public:
		virtual ~IScene() noexcept = default;

		virtual void Init(std::shared_ptr<IResourceManager> resourceManager) = 0;

		virtual void OnUpdate(const float deltaTime) = 0;
		virtual void OnRender(FrameDescription& renderer) = 0;

		virtual void Add(const GameObjectHandle handle) = 0;
		virtual void AddChild(const GameObjectHandle parentHandle, const GameObjectHandle child) = 0;
	};
}