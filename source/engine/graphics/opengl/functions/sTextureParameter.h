﻿#pragma once

#include <glad/glad.h>

#include "engine/graphics/opengl/assets/cTexture_opengl.h"

namespace df::opengl
{
	struct sTextureParameter
	{
		sTextureParameter() = delete;

		enum eName
		{
			kDepthStencilMode = GL_DEPTH_STENCIL_TEXTURE_MODE,
			kBaseLevel        = GL_TEXTURE_BASE_LEVEL,
			kCompareFunc      = GL_TEXTURE_COMPARE_FUNC,
			kCompareMode      = GL_TEXTURE_COMPARE_MODE,
			kLodBias          = GL_TEXTURE_LOD_BIAS,
			kMinFilter        = GL_TEXTURE_MIN_FILTER,
			kMagFilter        = GL_TEXTURE_MAG_FILTER,
			kMinLod           = GL_TEXTURE_MIN_LOD,
			kMaxLod           = GL_TEXTURE_MAX_LOD,
			kMaxLevel         = GL_TEXTURE_MAX_LEVEL,
			kSwizzleR         = GL_TEXTURE_SWIZZLE_R,
			kSwizzleG         = GL_TEXTURE_SWIZZLE_G,
			kSwizzleB         = GL_TEXTURE_SWIZZLE_B,
			kSwizzleA         = GL_TEXTURE_SWIZZLE_A,
			kWrapS            = GL_TEXTURE_WRAP_S,
			kWrapT            = GL_TEXTURE_WRAP_T,
			kWrapR            = GL_TEXTURE_WRAP_R,
			kBorderColor      = GL_TEXTURE_BORDER_COLOR,
			kSwizzleRgba      = GL_TEXTURE_SWIZZLE_RGBA
		};

		static void setInteger( const cTexture_opengl* _texture, const eName _name, const int _param ) { glTexParameteri( _texture->m_type, _name, _param ); }
		static void setInteger( const cTexture_opengl* _texture, const eName _name, int _param[ 4 ] ) { glTexParameteriv( _texture->m_type, _name, _param ); }

		template< eName T >
		struct sValue;

		typedef sValue< kDepthStencilMode > sDepthStencilMode;
		typedef sValue< kCompareFunc >      sCompareFunc;
		typedef sValue< kCompareMode >      sCompareMode;
		typedef sValue< kMinFilter >        sMinFilter;
		typedef sValue< kMagFilter >        sMagFilter;
		typedef sValue< kSwizzleR >         sSwizzleR;
		typedef sValue< kSwizzleG >         sSwizzleG;
		typedef sValue< kSwizzleB >         sSwizzleB;
		typedef sValue< kSwizzleA >         sSwizzleA;
		typedef sValue< kSwizzleRgba >      sSwizzleRgba;
		typedef sValue< kWrapS >            sWrapS;
		typedef sValue< kWrapT >            sWrapT;
		typedef sValue< kWrapR >            sWrapR;
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kDepthStencilMode >
	{
		enum eEnum
		{
			kDepthComponent = GL_DEPTH_COMPONENT,
			kStencilIndex   = GL_STENCIL_INDEX,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kCompareFunc >
	{
		enum eEnum
		{
			kLessEqual    = GL_LEQUAL,
			kGreaterEqual = GL_GEQUAL,
			kLess         = GL_LESS,
			kGreater      = GL_GREATER,
			kEqual        = GL_EQUAL,
			kNotEqual     = GL_NOTEQUAL,
			kAlways       = GL_ALWAYS,
			kNever        = GL_NEVER,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kCompareMode >
	{
		enum eEnum
		{
			kCompareRefToTexture = GL_COMPARE_REF_TO_TEXTURE,
			kNone                = GL_NONE,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kMinFilter >
	{
		enum eEnum
		{
			kNearest              = GL_NEAREST,
			kLinear               = GL_LINEAR,
			kNearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
			kLinearMipmapNearest  = GL_LINEAR_MIPMAP_NEAREST,
			kNearestMipmapLinear  = GL_NEAREST_MIPMAP_LINEAR,
			kLinearMipmapLinear   = GL_LINEAR_MIPMAP_LINEAR,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kMagFilter >
	{
		enum eEnum
		{
			kNearest = GL_NEAREST,
			kLinear  = GL_LINEAR,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kSwizzleR >
	{
		enum eEnum
		{
			kRed   = GL_RED,
			kGreen = GL_GREEN,
			kBlue  = GL_BLUE,
			kAlpha = GL_ALPHA,
			kZero  = GL_ZERO,
			kOne   = GL_ONE,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kSwizzleG >
	{
		enum eEnum
		{
			kRed   = GL_RED,
			kGreen = GL_GREEN,
			kBlue  = GL_BLUE,
			kAlpha = GL_ALPHA,
			kZero  = GL_ZERO,
			kOne   = GL_ONE,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kSwizzleB >
	{
		enum eEnum
		{
			kRed   = GL_RED,
			kGreen = GL_GREEN,
			kBlue  = GL_BLUE,
			kAlpha = GL_ALPHA,
			kZero  = GL_ZERO,
			kOne   = GL_ONE,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kSwizzleA >
	{
		enum eEnum
		{
			kRed   = GL_RED,
			kGreen = GL_GREEN,
			kBlue  = GL_BLUE,
			kAlpha = GL_ALPHA,
			kZero  = GL_ZERO,
			kOne   = GL_ONE,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kSwizzleRgba >
	{
		enum eEnum
		{
			kRed   = GL_RED,
			kGreen = GL_GREEN,
			kBlue  = GL_BLUE,
			kAlpha = GL_ALPHA,
			kZero  = GL_ZERO,
			kOne   = GL_ONE,
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kWrapS >
	{
		enum eEnum
		{
			kClampToEdge       = GL_CLAMP_TO_EDGE,
			kClampToBorder     = GL_CLAMP_TO_BORDER,
			kMirroredRepeat    = GL_MIRRORED_REPEAT,
			kRepeat            = GL_REPEAT,
			kMirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kWrapT >
	{
		enum eEnum
		{
			kClampToEdge       = GL_CLAMP_TO_EDGE,
			kClampToBorder     = GL_CLAMP_TO_BORDER,
			kMirroredRepeat    = GL_MIRRORED_REPEAT,
			kRepeat            = GL_REPEAT,
			kMirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
		};
	};

	template<>
	struct sTextureParameter::sValue< sTextureParameter::kWrapR >
	{
		enum eEnum
		{
			kClampToEdge       = GL_CLAMP_TO_EDGE,
			kClampToBorder     = GL_CLAMP_TO_BORDER,
			kMirroredRepeat    = GL_MIRRORED_REPEAT,
			kRepeat            = GL_REPEAT,
			kMirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
		};
	};
}