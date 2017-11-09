#include "SkeletonResourceLoader.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"

#include "Skeleton.h"
#include "../VeritasEngineBase/ResourceHandle.h"


VeritasEngine::SkeletonResourceLoader::SkeletonResourceLoader()
{
}

VeritasEngine::SkeletonResourceLoader::~SkeletonResourceLoader() = default;

const char* VeritasEngine::SkeletonResourceLoader::GetExtension() const
{
	return ".vesk";
}

void VeritasEngine::SkeletonResourceLoader::LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	cereal::BinaryInputArchive archive(data);

	Skeleton skeleton;
	archive(skeleton);

	handle.SetData(std::move(skeleton));
}
