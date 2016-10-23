#ifndef H_MESHNODE
#define H_MESHNODE

#include <memory>
#include <vector>

#include "../VeritasEngineBase/MathTypes.h"
#include "SmallObject.h"

namespace VeritasEngine
{
	struct SerializedMeshNode;

	class MeshNode : public SmallObject<>
	{
	public:
		MeshNode();
		~MeshNode();
		MeshNode(SerializedMeshNode& serializedNode);

		const Matrix4x4& GetTransform() const;
		void SetTransform(const Matrix4x4& transform);

		const std::vector<unsigned int>& GetMeshIndices() const;
		void SetMeshIndices(const std::vector<unsigned int>& meshIndices);

		const std::vector<MeshNode>& GetChildren() const;
		void AddChild(MeshNode&& child);

		MeshNode(const MeshNode& rhs);
		MeshNode& operator=(const MeshNode& rhs);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif