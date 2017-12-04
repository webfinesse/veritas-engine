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

// from nvtt.h

/** Return the next power of two.
* @see http://graphics.stanford.edu/~seander/bithacks.html
* @warning Behaviour for 0 is undefined.
* @note isPowerOfTwo(x) == true -> nextPowerOfTwo(x) == x
* @note nextPowerOfTwo(x) = 2 << log2(x-1)
*/
inline uint nextPowerOfTwo(uint x)
{
	nvDebugCheck(x != 0);
#if 1	// On modern CPUs this is supposed to be as fast as using the bsr instruction.
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x + 1;
#else
	uint p = 1;
	while (x > p) {
		p += p;
	}
	return p;
#endif
}

VeritasACP::ExportTextureResult VeritasACP::ExportTexture::Export(fs::path& fileName, bool isNormalMap)
{
	nvtt::InputOptions inputOptions;
	nv::Image image;
	ExportTextureResult result;

	image.load(fileName.generic_string().c_str());

	result.OriginalHeight = image.height();
	result.OriginalWidth = image.width();

	inputOptions.setNormalMap(isNormalMap);
	inputOptions.setRoundMode(nvtt::RoundMode_ToNextPowerOfTwo);	
	inputOptions.setTextureLayout(nvtt::TextureType_2D, image.width(), image.height());
	inputOptions.setMipmapData(image.pixels(), image.width(), image.height());

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

	result.NewHeight = nextPowerOfTwo(image.height());
	result.NewWidth = nextPowerOfTwo(image.width());

	VeritasACP::ACPState::Instance().AddAsset(resourceName);

	result.ResourceName = ACPState::Instance().GetResourceIdGenerator().GenerateId(resourceName);

	return result;
}