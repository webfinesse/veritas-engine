#ifndef H_DESERIALIZEMAPPING
#define H_DESERIALIZEMAPPING

#include <memory>

#include "IDeserializeMapping.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class DeserializeMapping: public IDeserializeMapping
	{
	public:
		DeserializeMapping();
		~DeserializeMapping() override;

		void Register(const StringHash jsonTag, const DESERIALIZEFUNCTION function) override;
		DESERIALIZEFUNCTION* const GetDeserializer(const StringHash jsonTag) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)

#endif