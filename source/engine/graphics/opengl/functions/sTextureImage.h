#pragma once

#include <glad/glad.h>

#include <engine/graphics/opengl/assets/cTexture_opengl.h>

#include "core/math/cVector.h"

namespace df::opengl
{
	struct sTextureImage
	{
		sTextureImage() = delete;

		enum eName
		{
			kInternalFormat,
			kFormat,
		};

		static void set2D( const cTexture_opengl* _texture,
		                   const int              _level,
		                   const int              _internal_format,
		                   const cVector2i&       _size,
		                   const int              _border,
		                   const int              _format,
		                   const unsigned         _type,
		                   const void*            _pixels )
		{
			glTexImage2D( _texture->m_type, _level, _internal_format, _size.x(), _size.y(), _border, _format, _type, _pixels );
		}

		template< eName T >
		struct sValue;

		typedef sValue< kInternalFormat > sInternalFormat;
		typedef sValue< kFormat >         sFormat;
	};

	template<>
	struct sTextureImage::sValue< sTextureImage::kInternalFormat >
	{
		struct Base
		{
			enum eEnum
			{
				kDepthComponent = GL_DEPTH_COMPONENT,
				kDepthStencil   = GL_DEPTH_STENCIL,
				kRed            = GL_RED,
				kRG             = GL_RG,
				kRGB            = GL_RGB,
				kRGBA           = GL_RGBA
			};
		};

		struct Sized
		{
			enum eEnum
			{
				kR8             = GL_R8,
				kR8SNorm        = GL_R8_SNORM,
				kR16            = GL_R16,
				kR16SNorm       = GL_R16_SNORM,
				kRG8            = GL_RG8,
				kRG8SNorm       = GL_RG8_SNORM,
				kRG16           = GL_RG16,
				kRG16SNorm      = GL_RG16_SNORM,
				kR3_G3_B2       = GL_R3_G3_B2,
				kRGB4           = GL_RGB4,
				kRGB5           = GL_RGB5,
				kRGB8           = GL_RGB8,
				kRGB8SNorm      = GL_RGB8_SNORM,
				kRGB10          = GL_RGB10,
				kRGB12          = GL_RGB12,
				kRGB16SNorm     = GL_RGB16_SNORM,
				kRGBA2          = GL_RGBA2,
				kRGBA4          = GL_RGBA4,
				kRGB5_A1        = GL_RGB5_A1,
				kRGBA8          = GL_RGBA8,
				kRGBA8SNorm     = GL_RGBA8_SNORM,
				kRGB10_A2       = GL_RGB10_A2,
				kRGB10_A2UI     = GL_RGB10_A2UI,
				kRGBA12         = GL_RGBA12,
				kRGBA16         = GL_RGBA16,
				kSRGB8          = GL_SRGB8,
				kSRGB8_ALPHA8   = GL_SRGB8_ALPHA8,
				kR16F           = GL_R16F,
				kRG16F          = GL_RG16F,
				kRGB16F         = GL_RGB16F,
				kRGBA16F        = GL_RGBA16F,
				kR32F           = GL_R32F,
				kRG32F          = GL_RG32F,
				kRGB32F         = GL_RGB32F,
				kRGBA32F        = GL_RGBA32F,
				kR11F_G11F_B10F = GL_R11F_G11F_B10F,
				kRGB9_E5        = GL_RGB9_E5,
				kR8I            = GL_R8I,
				kR8UI           = GL_R8UI,
				kR16I           = GL_R16I,
				kR16UI          = GL_R16UI,
				kR32I           = GL_R32I,
				kR32UI          = GL_R32UI,
				kRG8I           = GL_RG8I,
				kRG8UI          = GL_RG8UI,
				kRG16I          = GL_RG16I,
				kRG16UI         = GL_RG16UI,
				kRG32I          = GL_RG32I,
				kRG32UI         = GL_RG32UI,
				kRGB8I          = GL_RGB8I,
				kRGB8UI         = GL_RGB8UI,
				kRGB16I         = GL_RGB16I,
				kRGB16UI        = GL_RGB16UI,
				kRGB32I         = GL_RGB32I,
				kRGB32UI        = GL_RGB32UI,
				kRGBA8I         = GL_RGBA8I,
				kRGBA8UI        = GL_RGBA8UI,
				kRGBA16I        = GL_RGBA16I,
				kRGBA16UI       = GL_RGBA16UI,
				kRGBA32I        = GL_RGBA32I,
				kRGBA32UI       = GL_RGBA32UI,
			};
		};

		struct Compressed
		{
			enum eEnum
			{
				kR                = GL_COMPRESSED_RED,
				kRG               = GL_COMPRESSED_RG,
				kRGB              = GL_COMPRESSED_RGB,
				kRGBA             = GL_COMPRESSED_RGBA,
				kSRGB             = GL_COMPRESSED_SRGB,
				kSRGBA            = GL_COMPRESSED_SRGB_ALPHA,
				kR_RGTC1          = GL_COMPRESSED_RED_RGTC1,
				kSR_RGTC1         = GL_COMPRESSED_SIGNED_RED_RGTC1,
				kRG_RGTC2         = GL_COMPRESSED_RG_RGTC2,
				kSRG_RGTC2        = GL_COMPRESSED_SIGNED_RG_RGTC2,
				kRGBA_BPTC_UNorm  = GL_COMPRESSED_RGBA_BPTC_UNORM,
				kSRGBA_BPTC_UNorm = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
				kRGB_BPTC_SFloat  = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
				kRGB_BPTC_UFloat  = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
			};
		};
	};

	template<>
	struct sTextureImage::sValue< sTextureImage::kFormat >
	{
		enum eEnum
		{
			kRed            = GL_RED,
			kRG             = GL_RG,
			kRGB            = GL_RGB,
			kBGR            = GL_BGR,
			kRGBA           = GL_RGBA,
			kBGRA           = GL_BGRA,
			kRedInteger     = GL_RED_INTEGER,
			kRGInteger      = GL_RG_INTEGER,
			kRGBInteger     = GL_RGB_INTEGER,
			kBGRInteger     = GL_BGR_INTEGER,
			kRGBAInteger    = GL_RGBA_INTEGER,
			kBGRAInteger    = GL_BGRA_INTEGER,
			kStencilIndex   = GL_STENCIL_INDEX,
			kDepthComponent = GL_DEPTH_COMPONENT,
			kDepthStencil   = GL_DEPTH_STENCIL
		};
	};
}