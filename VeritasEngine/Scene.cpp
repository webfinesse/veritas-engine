#include "Scene.h"

#include <vector>
#include <array>
#include "../Includes/AssocVector/assocvector.hpp"

#include "IRenderer.h"
#include "MatrixStack.h"
#include "MeshInstance.h"
#include "MeshNode.h"
#include "IMeshShader.h"
#include "RenderPass.h"
#include "MeshInstance.h"
#include "MeshSubset.h"

#include "../VeritasEngineBase/MathTypes.h"
#include "../VeritasEngineBase/Material.h"
#include "../VeritasEngineBase/PerObjectBuffer.h"
#include "../VeritasEngineBase/ResourceHandle.h"
#include "../VeritasEngineBase/Light.h"
#include "GamePropertyManager.h"

#include "GameObjectPropertyKeys.h"


struct SceneNode : public VeritasEngine::SmallObject<>
{
	SceneNode(const VeritasEngine::GameObjectHandle handle)
		: m_handle(handle), m_children()
	{

	}

	VeritasEngine::GameObjectHandle m_handle;
	std::vector<SceneNode> m_children;
};

struct VeritasEngine::Scene::Impl
{
	Impl(std::shared_ptr<GamePropertyManager> gamePropertyManager)
		: m_cameraHandle(-1), m_lightHandles(), m_lightData(), m_mapping(), m_root(), m_matrixStack(), m_meshShader(nullptr), m_currentShader(nullptr), m_gamePropertyManager{ gamePropertyManager }
	{
		m_lightHandles.reserve(8);

		m_material.Ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_material.Diffuse = { 0.0f, 0.5f, 0.5f, 1.0f };
		m_material.Specular = { 1.0f, 0.0f, 0.0f, 10.0f };

		for (auto light : m_lightData)
		{
			light.Enabled = 0;
		}

		m_nodeType = gamePropertyManager->GetProperty<SceneNodeType>(GameObjectPropertyKeys::SceneNodeType);
		m_worldPosition = gamePropertyManager->GetProperty<Matrix4x4>(GameObjectPropertyKeys::WorldPosition);
		m_resourcedMesh = gamePropertyManager->GetProperty<ResourceHandle*>(GameObjectPropertyKeys::ResourcedMesh);
		m_objectMesh = gamePropertyManager->GetProperty<MeshInstance>(GameObjectPropertyKeys::ObjectMesh);
		m_cameraTarget = gamePropertyManager->GetProperty<Float3>(GameObjectPropertyKeys::CameraTarget);
		m_cameraPosition = gamePropertyManager->GetProperty<Float3>(GameObjectPropertyKeys::CameraPosition);
		m_light = gamePropertyManager->GetProperty<Light>(GameObjectPropertyKeys::Light);
	}

	void RenderResourcedMesh(const IRenderer& renderer, const MeshInstance& instance, const MeshNode& currentNode)
	{
		m_matrixStack.Push(currentNode.GetTransform());
		auto& stackMatrix = m_matrixStack.Peek();
		auto inverse = VeritasEngine::MathHelpers::Inverse(stackMatrix);
		auto inverseTranspose = VeritasEngine::MathHelpers::Transpose(inverse);

		for (const auto& meshIndex : currentNode.GetMeshIndices())
		{
			auto& subset = instance.GetSubset(meshIndex);
			const MaterialInstance& material = subset.GetMaterial()->GetData<MaterialInstance>();
			PerObjectBufferRef buffer(m_matrixStack.Peek(), inverseTranspose, material);
			m_meshShader->SetPerObjectBuffer(buffer);

			renderer.RenderSubset(instance, meshIndex);
		}

		for (const auto& item : currentNode.GetChildren())
		{
			RenderResourcedMesh(renderer, instance, item);
		}

		m_matrixStack.Pop();
	}

	void Render(const IRenderer& renderer, const SceneNode& node)
	{
		auto type = *m_nodeType->GetProperty(node.m_handle);

		auto matrix = m_worldPosition->GetProperty(node.m_handle);

		if (matrix != nullptr)
		{
			m_matrixStack.Push(*matrix);
		}

		switch (type)
		{
			case SceneNodeType::ResourcedMesh:
			{
				auto meshResource = m_resourcedMesh->GetProperty(node.m_handle);
				const MeshInstance& meshInstance = meshResource->GetData<MeshInstance>();

				auto& rootNode = meshInstance.GetRootNode();

				RenderResourcedMesh(renderer, meshInstance, rootNode);
				
				break;
			}

			case SceneNodeType::Mesh:
			{
				auto& stackMatrix = m_matrixStack.Peek();
				auto inverse = VeritasEngine::MathHelpers::Inverse(stackMatrix);
				auto inverseTranspose = VeritasEngine::MathHelpers::Transpose(inverse);

				PerObjectBufferRef buffer(m_matrixStack.Peek(), inverseTranspose, m_material);
				m_meshShader->SetPerObjectBuffer(buffer);

				auto mesh = m_objectMesh->GetProperty(node.m_handle);

				//renderer.RenderMesh(mesh);
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

	GameObjectHandle m_cameraHandle;
	std::vector<GameObjectHandle> m_lightHandles;
	std::array<Light, Light::MAX_LIGHTS> m_lightData;
	AssocVector<GameObjectHandle, SceneNode*> m_mapping;
	std::vector<SceneNode> m_root;
	MatrixStack m_matrixStack;
	Material m_material;
	std::shared_ptr<IMeshShader> m_meshShader;
	std::shared_ptr<IMeshShader> m_currentShader;

	std::shared_ptr<GamePropertyManager> m_gamePropertyManager;
	GameObjectProperty<SceneNodeType>* m_nodeType;
	GameObjectProperty<Matrix4x4>* m_worldPosition;
	GameObjectProperty<ResourceHandle*>* m_resourcedMesh;
	GameObjectProperty<MeshInstance>* m_objectMesh;
	GameObjectProperty<Float3>* m_cameraPosition;
	GameObjectProperty<Float3>* m_cameraTarget;
	GameObjectProperty<Light>* m_light;
};

VeritasEngine::Scene::Scene(std::shared_ptr<GamePropertyManager> gamePropertyManager)
	: m_impl(std::make_unique<Impl>(gamePropertyManager))
{

}

VeritasEngine::Scene::~Scene()
{
	m_impl->m_currentShader->Deactivate();
}

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

void VeritasEngine::Scene::SetMeshShader(std::shared_ptr<IMeshShader> shader)
{
	m_impl->m_meshShader = shader;
}

void VeritasEngine::Scene::OnRender(IRenderer& renderer)
{
	renderer.Clear();

	assert(m_impl->m_cameraHandle > 0);
	assert(m_impl->m_lightHandles.size() > 0);

	for (int i = RenderPass::Terrain; i != RenderPass::End; i++)
	{
		if (i == RenderPass::Mesh)
		{
			if (m_impl->m_currentShader != m_impl->m_meshShader)
			{
				m_impl->m_currentShader = m_impl->m_meshShader;
				m_impl->m_currentShader->Activate();
			}

			const auto& positionVec = *m_impl->m_cameraPosition->GetProperty(m_impl->m_cameraHandle);
			const auto& cameraTarget = *m_impl->m_cameraTarget->GetProperty(m_impl->m_cameraHandle);
			const Float3 cameraUp = { 0.0f, 1.0f, 0.0f };

			auto viewMatrix = MathHelpers::CreateLookAtMatrix(positionVec, cameraTarget, cameraUp);

			auto quarterPi = glm::quarter_pi<float>();
			auto aspectRatio = renderer.GetAspectRatio();
			auto near = 1.0f;
			auto far = 3000.0f;

			auto projectionMatrix = MathHelpers::CreatePerspectiveMatrix(quarterPi, aspectRatio, near, far);

			m_impl->m_meshShader->SetCameraParameters(positionVec, viewMatrix, projectionMatrix);

			for (size_t i = 0; i < m_impl->m_lightHandles.size(); i++)
			{
				auto light = m_impl->m_light->GetProperty(m_impl->m_lightHandles[i]);
				m_impl->m_lightData[i] = *light;
			}

			m_impl->m_meshShader->SetLightParameters(m_impl->m_lightData);

			for (const auto& sceneNode : m_impl->m_root)
			{
				m_impl->Render(renderer, sceneNode);
			}
		}
	}

	renderer.Present();
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