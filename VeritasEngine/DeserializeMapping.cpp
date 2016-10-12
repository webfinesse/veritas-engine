#include "DeserializeMapping.h"
#include <unordered_map>

struct VeritasEngine::DeserializeMapping::Impl
{
	static unordered_map<string, DESERIALIZEFUNCTION> m_mapping;
};

unordered_map<string, DESERIALIZEFUNCTION> VeritasEngine::DeserializeMapping::Impl::m_mapping{};

VeritasEngine::DeserializeMapping::DeserializeMapping()
	: m_impl(make_unique<Impl>())
{

}

VeritasEngine::DeserializeMapping::~DeserializeMapping() = default;

void VeritasEngine::DeserializeMapping::Register(const string& jsonTag, const DESERIALIZEFUNCTION function)
{
	m_impl->m_mapping.emplace(jsonTag, function);
}

DESERIALIZEFUNCTION* const VeritasEngine::DeserializeMapping::GetDeserializer(const string& jsonTag)
{
	auto query = m_impl->m_mapping.find(jsonTag);

	if (query != m_impl->m_mapping.end())
	{
		return &query->second;
	}

	return nullptr;
}