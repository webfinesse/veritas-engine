#ifndef H_SCENE
#define H_SCENE

#include <memory>
#include "../VeritasEngineBase/GameObjectHandle.h"
#include "SmallObject.h"
#include "DynamicLibraryHelper.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class Renderer;
	class IMeshShader;

	class EXPORT Scene: public SmallObject<>
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(const float deltaTime);
		void OnRender(Renderer& renderer);

		void SetMeshShader(std::shared_ptr<IMeshShader> shader);

		void Add(const GameObjectHandle handle);
		void AddChild(const GameObjectHandle parentHandle, const GameObjectHandle child);


	private:
		Scene(const Scene& rhs) = delete;
		Scene& operator=(const Scene& rhs) = delete;

		Scene(const Scene&& rhs) = delete;
		Scene& operator=(const Scene&& rhs) = delete;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)

#endif