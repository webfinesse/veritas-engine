#pragma once

#include <memory>

#include "DynamicLibraryHelper.h"
#include "../VeritasEngineBase/GameObjectHandle.h"

namespace VeritasEngine
{
	class IRenderer;
	class IMeshShader;

	class EXPORT IScene
	{
	public:
		virtual ~IScene() noexcept = default;

		virtual void OnUpdate(const float deltaTime) = 0;
		virtual void OnRender(IRenderer& renderer) = 0;

		virtual void SetMeshShader(std::shared_ptr<IMeshShader> shader) = 0;

		virtual void Add(const GameObjectHandle handle) = 0;
		virtual void AddChild(const GameObjectHandle parentHandle, const GameObjectHandle child) = 0;
	};
}