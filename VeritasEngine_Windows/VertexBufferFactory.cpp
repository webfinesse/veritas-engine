#include "VertexBufferFactory.h"
#include "VertexBufferImpl.h"
#include "DirectXState.h"

VeritasEngine::VertexBufferFactory::VertexBufferFactory(const std::shared_ptr<DirectXState> dxState)
	: m_dxState{ dxState }
{

}

std::shared_ptr<VeritasEngine::IVertexBuffer> VeritasEngine::VertexBufferFactory::Create(const size_t sizeOfVertex)
{
	return std::make_shared<VertexBufferImpl>(m_dxState, sizeOfVertex);
}
