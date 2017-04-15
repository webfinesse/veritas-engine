#include "SkeletonResourceLoader.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"

#include "Skeleton.h"
#include "../VeritasEngineBase/ResourceHandle.h"


VeritasEngine::SkeletonResourceLoader::SkeletonResourceLoader()
{
}

VeritasEngine::SkeletonResourceLoader::~SkeletonResourceLoader() = default;

void VeritasEngine::SkeletonResourceLoader::LoadResource(ResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	cereal::BinaryInputArchive archive(data);

	Skeleton skeleton;
	archive(skeleton);

	handle.SetData(std::move(skeleton));
}
