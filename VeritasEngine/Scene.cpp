#include "Scene.h"

#include <vector>
#include "../Includes/AssocVector/assocvector.hpp"

#include "MatrixStack.h"
#include "MeshInstance.h"
#include "MeshNode.h"
#include "RenderPass.h"
#include "MeshSubset.h"
#include "IAnimationManager.h"

#include "FrameDescription.h"
#include "../VeritasEngineBase/MathTypes.h"
#include "../VeritasEngineBase/MaterialInstance.h"
#include "../VeritasEngineBase/ResourceHandle.h"
#include "../VeritasEngineBase/Light.h"
#include "GamePropertyManager.h"

#include "GameObjectPropertyKeys.h"

struct SceneNode : public VeritasEngine::SmallObject<>
{
	SceneNode(const VeritasEngine::GameObjectHandle handle)
		: m_handle{ handle }
	{

	}

	VeritasEngine::GameObjectHandle m_handle;
	std::vector<SceneNode> m_children{};
};

struct VeritasEngine::Scene::Impl
{
	Impl(std::shared_ptr<GamePropertyManager> gamePropertyManager, std::shared_ptr<IAnimationManager> animationManager)
		: m_gamePropertyManager{ gamePropertyManager }, m_animationManager { animationManager }
	{
		m_lightHandles.reserve(8);

		m_objectMesh = gamePropertyManager->RegisterProperty<MeshInstance>("Object Mesh", GameObjectPropertyKeys::ObjectMesh);
		m_resourcedMesh = gamePropertyManager->RegisterProperty<ResourceHandle*>("ResourcedMesh", GameObjectPropertyKeys::ResourcedMesh);

		m_nodeType = gamePropertyManager->RegisterProperty<SceneNodeType>("Scene Node Type", GameObjectPropertyKeys::SceneNodeType);
		m_worldPosition = gamePropertyManager->RegisterProperty<Matrix4x4>("World Position", GameObjectPropertyKeys::WorldPosition);

		m_cameraTarget = gamePropertyManager->RegisterProperty<Float3>("Camera Target", GameObjectPropertyKeys::CameraTarget);
		m_cameraPosition = gamePropertyManager->RegisterProperty<Float3>("Camera Position", GameObjectPropertyKeys::CameraPosition);

		m_light = gamePropertyManager->RegisterProperty<Light>("Light", GameObjectPropertyKeys::Light);
	}

	void RenderResourcedMesh(FrameDescription& renderer, const SceneNode& sceneNode, const MeshInstance& instance, const SceneNodeType nodeType, const MeshNode& currentNode)
	{
		m_matrixStack.Push(currentNode.GetTransform());
		auto& stackMatrix = m_matrixStack.Peek();
		auto inverse = VeritasEngine::MathHelpers::Inverse(stackMatrix);
		auto inverseTranspose = VeritasEngine::MathHelpers::Transpose(inverse);

		for (const auto& meshIndex : currentNode.GetMeshIndices())
		{
			auto& subset = instance.GetSubset(meshIndex);
			const MaterialInstance& material = subset.GetMaterial()->GetData<MaterialInstance>();

			renderer.StaticObjects.emplace_back(stackMatrix, inverseTranspose, &material, instance.GetIndexBuffer().GetNativeBuffer(),
				subset.GetIndexBufferIndicies(), instance.GetIndexBufferStartIndex(), instance.GetVertexBuffer().GetNativeBuffer(), subset.GetVertexBufferIndicies(), instance.GetVertexSize(), instance.GetVertexBufferStartIndex());
			
		}

		for (const auto& item : currentNode.GetChildren())
		{
			RenderResourcedMesh(renderer, sceneNode, instance, nodeType, item);
		}

		m_matrixStack.Pop();
	}

	void RenderAnimatedResourcedMesh(FrameDescription& renderer, const SceneNode& sceneNode, const MeshInstance& instance, const SceneNodeType nodeType, const MeshNode& currentNode)
	{
		auto& stackMatrix = m_matrixStack.Peek();
		auto inverse = VeritasEngine::MathHelpers::Inverse(stackMatrix);
		auto inverseTranspose = VeritasEngine::MathHelpers::Transpose(inverse);

		for (const auto& meshIndex : currentNode.GetMeshIndices())
		{
			auto& subset = instance.GetSubset(meshIndex);
			const MaterialInstance& material = subset.GetMaterial()->GetData<MaterialInstance>();

			renderer.AnimatedObjects.emplace_back(stackMatrix, inverseTranspose, &material, instance.GetIndexBuffer().GetNativeBuffer(),
				subset.GetIndexBufferIndicies(), instance.GetIndexBufferStartIndex(), instance.GetVertexBuffer().GetNativeBuffer(), subset.GetVertexBufferIndicies(), instance.GetVertexSize(), instance.GetVertexBufferStartIndex(), m_animationManager->GetSkinningPalette(sceneNode.m_handle));
		}

		for (const auto& item : currentNode.GetChildren())
		{
			RenderAnimatedResourcedMesh(renderer, sceneNode, instance, nodeType, item);
		}
	}

	void Render(FrameDescription& renderer, const SceneNode& node)
	{
		const auto type = *m_nodeType->GetProperty(node.m_handle);

		const auto matrix = m_worldPosition->GetProperty(node.m_handle);

		if (matrix != nullptr)
		{
			m_matrixStack.Push(*matrix);
		}

		switch (type)
		{
			case SceneNodeType::ResourcedMesh:
			{
				const auto meshResource = m_resourcedMesh->GetProperty(node.m_handle);
				const MeshInstance& meshInstance = meshResource->GetData<MeshInstance>();

				auto& rootNode = meshInstance.GetRootNode();

				RenderResourcedMesh(renderer, node, meshInstance, type, rootNode);
				
				break;
			}
			case SceneNodeType::AnimatedResourcedMesh:
			{
				const auto meshResource = m_resourcedMesh->GetProperty(node.m_handle);
				const MeshInstance& meshInstance = meshResource->GetData<MeshInstance>();

				auto& rootNode = meshInstance.GetRootNode();

				RenderAnimatedResourcedMesh(renderer, node, meshInstance, type, rootNode);

				break;
			}
		}

		for (const auto& child : node.m_children)
		{
			Render(renderer, child);
		}

		if (matrix != nullptr)
		{
			m_matrixStack.Pop();
		}
	}

	GameObjectHandle m_cameraHandle{ static_cast<GameObjectHandle>(-1) };
	std::vector<GameObjectHandle> m_lightHandles{};
	AssocVector<GameObjectHandle, SceneNode*> m_mapping{};
	std::vector<SceneNode> m_root{};
	MatrixStack m_matrixStack{};

	std::shared_ptr<GamePropertyManager> m_gamePropertyManager;
	std::shared_ptr<IAnimationManager> m_animationManager;
	GameObjectProperty<SceneNodeType>* m_nodeType;
	GameObjectProperty<Matrix4x4>* m_worldPosition;
	GameObjectProperty<ResourceHandle*>* m_resourcedMesh;
	GameObjectProperty<MeshInstance>* m_objectMesh;
	GameObjectProperty<Float3>* m_cameraPosition;
	GameObjectProperty<Float3>* m_cameraTarget;
	GameObjectProperty<Light>* m_light;
};

VeritasEngine::Scene::Scene(std::shared_ptr<GamePropertyManager> gamePropertyManager, std::shared_ptr<IAnimationManager> animationManager)
	: m_impl(std::make_unique<Impl>(gamePropertyManager, animationManager))
{

}

VeritasEngine::Scene::~Scene() = default;

VeritasEngine::Scene::Scene(Scene&& other) noexcept
	: m_impl { std::move(other.m_impl) }
{

}

VeritasEngine::Scene& VeritasEngine::Scene::operator=(Scene&& other) noexcept
{
	if(this != &other)
	{
		m_impl = std::move(other.m_impl);
	}

	return *this;
}

void VeritasEngine::Scene::OnUpdate(float deltaTime)
{

}

void VeritasEngine::Scene::OnRender(FrameDescription& renderer)
{
	assert(m_impl->m_cameraHandle > 0);
	assert(m_impl->m_lightHandles.size() > 0);

	for (int i = RenderPass::Terrain; i != RenderPass::End; i++)
	{
		if (i == RenderPass::Mesh)
		{
			const auto& positionVec = *m_impl->m_cameraPosition->GetProperty(m_impl->m_cameraHandle);
			const auto& cameraTarget = *m_impl->m_cameraTarget->GetProperty(m_impl->m_cameraHandle);
			const Float3 cameraUp = { 0.0f, 1.0f, 0.0f };

			renderer.PassBuffer.ViewMatrix = MathHelpers::CreateLookAtMatrix(positionVec, cameraTarget, cameraUp);

			auto quarterPi = glm::quarter_pi<float>();
			auto aspectRatio = renderer.AspectRatio;
			auto near = 1.0f;
			auto far = 3000.0f;

			// this is an opengl perspective matrix, change it to work for directx. https://www.gamedev.net/forums/topic/692095-d3d-glm-depth-reconstruction-issues/
			auto projectionMatrix = MathHelpers::CreatePerspectiveMatrix(quarterPi, aspectRatio, near, far);
			projectionMatrix = glm::scale(projectionMatrix, Float3(1.0f, 1.0f, 0.5f));
			renderer.PassBuffer.ProjectionMatrix = glm::translate(projectionMatrix, Float3(0.0f, 0.0f, 0.5f));

			renderer.PassBuffer.EyePosition = Float4(positionVec, 0);

			size_t i = 0;
			for (; i < m_impl->m_lightHandles.size(); i++)
			{
				auto light = m_impl->m_light->GetProperty(m_impl->m_lightHandles[i]);
				renderer.PassBuffer.Lights[i] = *light;
			}

			for(; i < Light::MAX_LIGHTS; i++)
			{
				renderer.PassBuffer.Lights[i].Enabled = 0;
			}

			for (const auto& sceneNode : m_impl->m_root)
			{
				m_impl->Render(renderer, sceneNode);
			}
		}
	}
}

void VeritasEngine::Scene::Add(const GameObjectHandle handle)
{
	auto existingChild = m_impl->m_mapping.find(handle);
	auto end = m_impl->m_mapping.end();

	assert(existingChild == end); // "handle already exists in the graph"

	auto ptr = std::make_shared<SceneNode>(handle);

	m_impl->m_root.emplace_back(handle);
	m_impl->m_mapping[handle] = &m_impl->m_root.back();

	auto type = *m_impl->m_nodeType->GetProperty(handle);

	if (type == SceneNodeType::Camera) 
	{
		m_impl->m_cameraHandle = handle;
	}

	if (type == SceneNodeType::Light)
	{
		m_impl->m_lightHandles.emplace_back(handle);
	}
}

void VeritasEngine::Scene::AddChild(const GameObjectHandle parentHandle, const GameObjectHandle child)
{
	auto item = m_impl->m_mapping.find(parentHandle);
	auto existingChild = m_impl->m_mapping.find(child);
	auto end = m_impl->m_mapping.end();

	assert(item != end); // "could not find the parent"
	assert(existingChild == end); // "child already exists in the graph"

	item->second->m_children.emplace_back(child);
	m_impl->m_mapping[child] = &item->second->m_children.back();
}