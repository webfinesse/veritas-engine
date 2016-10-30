#ifndef H_DESERIALIZEMAPPING
#define H_DESERIALIZEMAPPING

#include <functional>
#include <memory>

#include "../VeritasEngineBase/Singleton.h"
#include "../VeritasEngineBase/GameObjectHandle.h"
#include "JsonValue.h"
#include "DynamicLibraryHelper.h"
#include "StringHash.h"


using namespace std;
using DESERIALIZEFUNCTION = std::function<void(VeritasEngine::GameObjectHandle handle, VeritasEngine::JsonValue& values)>;

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class EXPORT DeserializeMapping : public Singleton<DeserializeMapping>
	{
		friend class Singleton<DeserializeMapping>;

	public:
		void Register(StringHash jsonTag, const DESERIALIZEFUNCTION function);
		DESERIALIZEFUNCTION* const GetDeserializer(StringHash jsonTag);

	private:
		DeserializeMapping();
		~DeserializeMapping();

		struct Impl;
		unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)

#endif