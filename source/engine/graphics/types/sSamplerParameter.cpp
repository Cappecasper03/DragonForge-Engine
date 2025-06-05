#include "sSamplerParameter.h"

#include <glad/glad.h>

namespace df
{

	int sSamplerParameter::toOpenGl( const eName _name )
	{
		switch( _name )
		{
			case kMinFilter:    return GL_TEXTURE_MIN_FILTER;
			case kMagFilter:    return GL_TEXTURE_MAG_FILTER;
			case kMipMapFilter: return 0;

			case kWrapS: return GL_TEXTURE_WRAP_S;
			case kWrapT: return GL_TEXTURE_WRAP_T;
			case kWrapR: return GL_TEXTURE_WRAP_R;
		}

		return 0;
	}

	int sSamplerParameter::toOpenGl( const eParameter _parameter )
	{
		switch( _parameter )
		{
			case kNearest: return GL_NEAREST;
			case kLinear:  return GL_LINEAR;

			case kNearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
			case kLinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
			case kNearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
			case kLinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;

			case kClampToEdge:       return GL_CLAMP_TO_EDGE;
			case kClampToBorder:     return GL_CLAMP_TO_BORDER;
			case kMirroredRepeat:    return GL_MIRRORED_REPEAT;
			case kRepeat:            return GL_REPEAT;
			case kMirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
		}

		return 0;
	}

	int sSamplerParameter::toVulkan( const eParameter _parameter )
	{
		switch( _parameter )
		{
			case kNearest: return static_cast< int >( vk::Filter::eNearest );
			case kLinear:  return static_cast< int >( vk::Filter::eLinear );

			case kNearestMipmapNearest:
			case kLinearMipmapNearest:  return static_cast< int >( vk::SamplerMipmapMode::eNearest );

			case kNearestMipmapLinear:
			case kLinearMipmapLinear:  return static_cast< int >( vk::SamplerMipmapMode::eLinear );

			case kClampToEdge:       return static_cast< int >( vk::SamplerAddressMode::eClampToEdge );
			case kClampToBorder:     return static_cast< int >( vk::SamplerAddressMode::eClampToBorder );
			case kMirroredRepeat:    return static_cast< int >( vk::SamplerAddressMode::eMirroredRepeat );
			case kRepeat:            return static_cast< int >( vk::SamplerAddressMode::eRepeat );
			case kMirrorClampToEdge: return static_cast< int >( vk::SamplerAddressMode::eMirrorClampToEdge );
		}

		return 0;
	}
}
