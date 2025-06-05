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

			case kRGB16f: return GL_RGB16F;
		}

		return 0;
	}

	vk::Format sTextureFormat::toVulkan( const eFormat _format )
	{
		DF_ProfilingScopeCpu;

		switch( _format )
		{
			case kRed:  return vk::Format::eR8Uint;
			case kRGB:  return vk::Format::eR8G8B8Uint;
			case kRGBA: return vk::Format::eR8G8B8A8Uint;

			case kRGB16f: return vk::Format::eR16G16B16A16Sfloat;
		}

		return vk::Format::eUndefined;
	}
}
