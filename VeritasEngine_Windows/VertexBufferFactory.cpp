#include "VertexBufferFactory.h"
#include "VertexBufferImpl.h"

std::shared_ptr<VeritasEngine::IVertexBuffer> VeritasEngine::VertexBufferFactory::Create(size_t sizeOfVertex)
{
	return std::make_shared<VertexBufferImpl>(sizeOfVertex);
}
