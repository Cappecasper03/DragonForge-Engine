#pragma once

#include <glad/glad.h>

namespace df::opengl
{
	enum eDataType
	{
		kByte          = GL_BYTE,
		kUnsignedByte  = GL_UNSIGNED_BYTE,
		kShort         = GL_SHORT,
		kUnsignedShort = GL_UNSIGNED_SHORT,
		kInt           = GL_INT,
		kUnsignedInt   = GL_UNSIGNED_INT,
		kFloat         = GL_FLOAT,
	};

	enum eEnableDisable
	{
		kBlend                      = GL_BLEND,
		kClipDistance               = GL_CLIP_DISTANCE0,
		kColorLogicOp               = GL_COLOR_LOGIC_OP,
		kCullFace                   = GL_CULL_FACE,
		kDebugOutput                = GL_DEBUG_OUTPUT,
		kDebugOutputSynchronous     = GL_DEBUG_OUTPUT_SYNCHRONOUS,
		kDepthClamp                 = GL_DEPTH_CLAMP,
		kDepthTest                  = GL_DEPTH_TEST,
		kDither                     = GL_DITHER,
		kFramebufferSrgb            = GL_FRAMEBUFFER_SRGB,
		kLineSmooth                 = GL_LINE_SMOOTH,
		kMultisample                = GL_MULTISAMPLE,
		kPolygonOffsetFill          = GL_POLYGON_OFFSET_FILL,
		kPolygonOffsetLine          = GL_POLYGON_OFFSET_LINE,
		kPolygonOffsetPoint         = GL_POLYGON_OFFSET_POINT,
		kPolygonSmooth              = GL_POLYGON_SMOOTH,
		kPrimitiveRestart           = GL_PRIMITIVE_RESTART,
		kPrimitiveRestartFixedIndex = GL_PRIMITIVE_RESTART_FIXED_INDEX,
		kRasterizerDiscard          = GL_RASTERIZER_DISCARD,
		kSampleAlphaToCoverage      = GL_SAMPLE_ALPHA_TO_COVERAGE,
		kSampleAlphaToOne           = GL_SAMPLE_ALPHA_TO_ONE,
		kSampleCoverage             = GL_SAMPLE_COVERAGE,
		kSampleShading              = GL_SAMPLE_SHADING,
		kSampleMask                 = GL_SAMPLE_MASK,
		kScissorTest                = GL_SCISSOR_TEST,
		kStencilTest                = GL_STENCIL_TEST,
		kTextureCubeMapSeamless     = GL_TEXTURE_CUBE_MAP_SEAMLESS,
		kProgramPointSize           = GL_PROGRAM_POINT_SIZE
	};

	enum eBlendingFactor
	{
		kZero                  = GL_ZERO,
		kOne                   = GL_ONE,
		kSrcColor              = GL_SRC_COLOR,
		kOneMinusSrcColor      = GL_ONE_MINUS_SRC_COLOR,
		kDstColor              = GL_DST_COLOR,
		kOneMinusDstColor      = GL_ONE_MINUS_DST_COLOR,
		kSrcAlpha              = GL_SRC_ALPHA,
		kOneMinusSrcAlpha      = GL_ONE_MINUS_SRC_ALPHA,
		kDstAlpha              = GL_DST_ALPHA,
		kOneMinusDstAlpha      = GL_ONE_MINUS_DST_ALPHA,
		kConstantColor         = GL_CONSTANT_COLOR,
		kOneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
		kConstantAlpha         = GL_CONSTANT_ALPHA,
		kOneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
		kSrcAlphaSaturate      = GL_SRC_ALPHA_SATURATE,
		kSrc1Color             = GL_SRC1_COLOR,
		kOneMinusSrc1Color     = GL_ONE_MINUS_SRC1_COLOR,
		kSrc1Alpha             = GL_SRC1_ALPHA,
		kOneMinusSrc1Alpha     = GL_ONE_MINUS_SRC1_ALPHA
	};

	enum ePrimitiveType
	{
		kPoints                 = GL_POINTS,
		kLineStrip              = GL_LINE_STRIP,
		kLineLoop               = GL_LINE_LOOP,
		kLines                  = GL_LINES,
		kLineStripAdjacency     = GL_LINE_STRIP_ADJACENCY,
		kLinesAdjacency         = GL_LINES_ADJACENCY,
		kTriangleStrip          = GL_TRIANGLE_STRIP,
		kTriangleFan            = GL_TRIANGLE_FAN,
		kTriangles              = GL_TRIANGLES,
		kTriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
		kTrianglesAdjacency     = GL_TRIANGLES_ADJACENCY,
		kPatches                = GL_PATCHES
	};
}