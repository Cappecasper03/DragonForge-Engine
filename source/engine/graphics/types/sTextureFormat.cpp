#include "sTextureFormat.h"

#include <glad/glad.h>

#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	int sTextureFormat::toOpenGl( const eFormat _format )
	{
		DF_ProfilingScopeCpu;

		switch( _format )
		{
			case kRed:  return GL_RED;
			case kRGB:  return GL_RGB;
			case kRGBA: return GL_RGBA;

			case kRed8:   return GL_R8;
			case kRGB8:   return GL_RGB8;
			case kRGBA8:  return GL_RGBA8;
			case kRGB16f: return GL_RGB16F;
		}

		return 0;
	}

	vk::Format sTextureFormat::toVulkan( const eFormat _format )
	{
		DF_ProfilingScopeCpu;

		switch( _format )
		{
			case kRed:
			case kRed8: return vk::Format::eR8Unorm;

			case kRGB:
			case kRGB8: return vk::Format::eR8G8B8Unorm;

			case kRGBA:
			case kRGBA8: return vk::Format::eR8G8B8A8Unorm;

			case kRGB16f: return vk::Format::eR16G16B16Sfloat;
		}

		return vk::Format::eUndefined;
	}
}
