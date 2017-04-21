#ifndef H_WORLDSETUP
#define H_WORLDSETUP

#include <memory>

#include "IWorldSetup.h"

namespace VeritasEngine
{
	class IDeserializeMapping;

	class WorldSetup : public IWorldSetup
	{
	public:
		WorldSetup(std::shared_ptr<IDeserializeMapping> deserializeMapping);
		WorldSetup(WorldSetup&& other) noexcept;
		WorldSetup& operator=(WorldSetup&& other) noexcept;
		~WorldSetup() override;

		void Init(Engine& engine) override;
		void LoadFile(const char* filename) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif