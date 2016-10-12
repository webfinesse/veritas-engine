#include "MeshShader.h"
#include "../VeritasEngineBase/MeshShaderImpl.h"

struct VeritasEngine::MeshShader::Impl
{
	Impl()
		: m_shader{}
	{

	}

	MeshShaderImpl m_shader;
};

VeritasEngine::MeshShader::MeshShader()
	: m_impl{ std::make_unique<Impl>() }
{

}

VeritasEngine::MeshShader::~MeshShader() = default;

void VeritasEngine::MeshShader::Activate()
{
	m_impl->m_shader.Activate();
}

void VeritasEngine::MeshShader::Deactivate()
{
	m_impl->m_shader.Deactivate();
}

void VeritasEngine::MeshShader::Init()
{
	m_impl->m_shader.Init();
}

void VeritasEngine::MeshShader::SetLightParameters(const std::array<VeritasEngine::Light, Light::MAX_LIGHTS>& lightsArray)
{
	m_impl->m_shader.SetLightParameters(lightsArray);
}

void VeritasEngine::MeshShader::SetCameraParameters(const Float3& eyePosition, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix)
{
	m_impl->m_shader.SetCameraParameters(eyePosition, viewMatrix, projectionMatrix);
}

void VeritasEngine::MeshShader::SetPerObjectBuffer(const PerObjectBufferRef& buffer)
{
	m_impl->m_shader.SetPerObjectBuffer(buffer);
}