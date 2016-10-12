#ifndef H_SCENENODE
#define H_SCENENODE

#include <memory>

#include "../VeritasEngineBase/GameObjectHandle.h"
#include "../VeritasEngineBase/SmallObject.h"

namespace VeritasEngine {
	class SceneNode : public SmallObject<>
	{
		friend class Scene;

	public:
		SceneNode(const GameObjectHandle handle);
		~SceneNode();
		SceneNode(const SceneNode& rhs);
		SceneNode& operator=(const SceneNode& rhs);

		const GameObjectHandle GetHandle() const;


	private:
		void AddChild(const SceneNode& handle);
		void RemoveChild(const GameObjectHandle handle);

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif