#include "DeserializeMapping.h"
#include "../Includes/AssocVector/AssocVector.hpp"

struct VeritasEngine::DeserializeMapping::Impl
{
	AssocVector<StringHash, DESERIALIZEFUNCTION> m_mapping{};
};

VeritasEngine::DeserializeMapping::DeserializeMapping()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasEngine::DeserializeMapping::~DeserializeMapping() = default;

void VeritasEngine::DeserializeMapping::Register(const StringHash jsonTag, const DESERIALIZEFUNCTION function)
{
	m_impl->m_mapping.emplace(jsonTag, function);
}

VeritasEngine::DESERIALIZEFUNCTION* const VeritasEngine::DeserializeMapping::GetDeserializer(const StringHash jsonTag)
{
	const auto query = m_impl->m_mapping.find(jsonTag);

	if (query != m_impl->m_mapping.end())
	{
		return &query->second;
	}

	return nullptr;
}