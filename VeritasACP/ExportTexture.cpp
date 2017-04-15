#include "ExportTexture.h"
#include "ACPState.h"
#include "ResourceIdGenerator.h"

#include <fstream>
#include <algorithm>
#include <iterator>

#include <boost/filesystem.hpp>

#include "../Includes/nvtt/include/nvtt/nvtt.h"
#include "../Includes/nvtt/include/nvtt/nvtt_wrapper.h"

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

VeritasEngine::ResourceId VeritasACP::ExportTexture::Export(fs::path& fileName)
{
	VeritasEngine::ResourceId textureInfo;

	nvtt::InputOptions inputOptions;
	nvtt::Surface image;

	image.load(fileName.generic_string().c_str());
	image.setAlphaMode(image.alphaMode());

	inputOptions.setTextureLayout(nvtt::TextureType_2D, image.width(), image.height());

	nvtt::CompressionOptions compressionOptions;
	compressionOptions.setFormat(nvtt::Format_BC3);
	compressionOptions.setQuality(nvtt::Quality_Normal);

	nvtt::Context context;
	context.enableCudaAcceleration(true);

	context.estimateSize(image, 1, compressionOptions);

	auto resourceName = fileName.replace_extension(".dds");

	nvtt::OutputOptions outputOptions;
	outputOptions.setFileName(resourceName.generic_string().c_str());

	MyErrorHandler handler;
	outputOptions.setErrorHandler(&handler);

	context.outputHeader(image, 1, compressionOptions, outputOptions);
	context.compress(image, 0, 0, compressionOptions, outputOptions);	

	VeritasACP::ACPState::Instance().GetAssetList().push_back(resourceName);

	textureInfo = ACPState::Instance().GetResourceIdGenerator().GenerateId(resourceName);

	return textureInfo;
}