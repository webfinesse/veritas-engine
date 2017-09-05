#include "ExportTexture.h"
#include "ACPState.h"
#include "ResourceIdGenerator.h"

#include <boost/filesystem.hpp>

#include "../Includes/nvtt/include/nvimage/Image.h"
#include "../Includes/nvtt/include/nvtt/nvtt.h"

struct VeritasACP::ExportTexture::Impl
{

};

VeritasACP::ExportTexture::ExportTexture()
	: m_impl(std::make_unique<Impl>())
{

}

VeritasACP::ExportTexture::~ExportTexture()
{

}

struct MyErrorHandler : public nvtt::ErrorHandler
{
	virtual void error(nvtt::Error e)
	{
#if _DEBUG
		__debugbreak();
#endif
		printf("Error: '%s'\n", nvtt::errorString(e));
	}
};

VeritasEngine::ResourceId VeritasACP::ExportTexture::Export(fs::path& fileName, bool isNormalMap)
{
	nvtt::InputOptions inputOptions;
	nv::Image image;

	image.load(fileName.generic_string().c_str());

	inputOptions.setNormalMap(isNormalMap);
	inputOptions.setRoundMode(nvtt::RoundMode_ToPreviousPowerOfTwo);	
	inputOptions.setTextureLayout(nvtt::TextureType_2D, image.width(), image.height());
	auto result = inputOptions.setMipmapData(image.pixels(), image.width(), image.height());

	nvtt::CompressionOptions compressionOptions;
	compressionOptions.setFormat(nvtt::Format_BC3);
	compressionOptions.setQuality(nvtt::Quality_Normal);

	auto fileNameCopy = fileName;
	auto resourceName = fileNameCopy.replace_extension(".dds");

	nvtt::OutputOptions outputOptions;
	outputOptions.setFileName(resourceName.generic_string().c_str());

	MyErrorHandler handler;
	outputOptions.setErrorHandler(&handler);

	nvtt::Context context;
	context.enableCudaAcceleration(true);

	context.process(inputOptions, compressionOptions, outputOptions);

	VeritasACP::ACPState::Instance().AddAsset(resourceName);

	VeritasEngine::ResourceId textureInfo = ACPState::Instance().GetResourceIdGenerator().GenerateId(resourceName);

	return textureInfo;
}