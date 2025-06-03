#include "sTextureFormat.h"

#include <glad/glad.h>

int df::sTextureFormat::toOpenGl( const eFormat _format )
{
	switch( _format )
	{
		case kRed:  return GL_RED;
		case kRGB:  return GL_RGB;
		case kRGBA: return GL_RGBA;

		case kRGB16f: return GL_RGB16F;
	}

	return 0;
}