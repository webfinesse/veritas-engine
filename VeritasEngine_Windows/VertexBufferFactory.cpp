#include "VertexBufferFactory.h"
#include "VertexBufferImpl.h"
#include "DirectXState.h"

VeritasEngine::VertexBufferFactory::VertexBufferFactory(std::shared_ptr<DirectXState> dxState)
	: m_dxState{ std::move(dxState) }
{

}

std::shared_ptr<VeritasEngine::IVertexBuffer> VeritasEngine::VertexBufferFactory::Create(const size_t sizeOfVertex)
{
	return std::make_shared<VertexBufferImpl>(m_dxState, sizeOfVertex);
}
