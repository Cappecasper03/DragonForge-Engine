#include "sTextureFormat.h"

#include <glad/glad.h>

#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	int sTextureFormat::toOpenGlInternal( const eFormat _format )
	{
		DF_ProfilingScopeCpu;

		switch( _format )
		{
			case kRed:  return GL_R8;
			case kRGB:  return GL_RGB8;
			case kRGBA: return GL_RGBA8;

			case kRGB16sf:  return GL_RGB16F;
			case kRGBA16sf: return GL_RGBA16F;
		}

		return 0;
	}

	int sTextureFormat::toOpenGlBase( const eFormat _format )
	{
		DF_ProfilingScopeCpu;

		switch( _format )
		{
			case kRed: return GL_RED;

			case kRGB:
			case kRGB16sf: return GL_RGB;

			case kRGBA:
			case kRGBA16sf: return GL_RGBA;
		}

		return 0;
	}

	vk::Format sTextureFormat::toVulkan( const eFormat _format )
	{
		DF_ProfilingScopeCpu;

		switch( _format )
		{
			case kRed:  return vk::Format::eR8Unorm;
			case kRGB:  return vk::Format::eR8G8B8Unorm;
			case kRGBA: return vk::Format::eR8G8B8A8Unorm;

			case kRGB16sf:  return vk::Format::eR16G16B16Sfloat;
			case kRGBA16sf: return vk::Format::eR16G16B16A16Sfloat;
		}

		return vk::Format::eUndefined;
	}

	unsigned sTextureFormat::bytesPerPixel( const eFormat _format )
	{
		DF_ProfilingScopeCpu;

		switch( _format )
		{
			case kRed: return 1;

			case kRGB:
			case kRGB16sf: return 3;

			case kRGBA:
			case kRGBA16sf: return 4;
		}

		return 0;
	}
}
