#include "SkeletonResourceLoader.h"

#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"

#include "Skeleton.h"
#include "ResourceData.h"

constexpr char extension[] = ".vesk";

const char* VeritasEngine::SkeletonResourceLoader::GetExtension() const
{
	return extension;
}

VeritasEngine::StringHash VeritasEngine::SkeletonResourceLoader::GetExtensionHash() const
{
	constexpr auto hash = CompileTimeHash(extension);
	return hash;
}

void VeritasEngine::SkeletonResourceLoader::LoadResource(IResourceManager& manager, Job* parentJob, std::istream& data, ResourceData& handle)
{
	cereal::BinaryInputArchive archive(data);

	Skeleton skeleton;
	archive(skeleton);

	handle.SetData(std::move(skeleton));
}
