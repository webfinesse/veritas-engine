#include "SkeletonResourceLoader.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"

#include "Skeleton.h"
#include "../VeritasEngineBase/ResourceHandle.h"

constexpr char extension[] = ".vesk";

VeritasEngine::SkeletonResourceLoader::SkeletonResourceLoader()
{
}

VeritasEngine::SkeletonResourceLoader::~SkeletonResourceLoader() = default;

const char* VeritasEngine::SkeletonResourceLoader::GetExtension() const
{
	return extension;
}

VeritasEngine::StringHash VeritasEngine::SkeletonResourceLoader::GetExtensionHash() const
{
	constexpr auto hash = CompileTimeHash(extension);
	return hash;
}

void VeritasEngine::SkeletonResourceLoader::LoadResource(IResourceManager& manager, std::istream& data, ResourceHandle& handle)
{
	cereal::BinaryInputArchive archive(data);

	Skeleton skeleton;
	archive(skeleton);

	handle.SetData(std::move(skeleton));
}
