#pragma once

#include "engine/misc/Misc.h"
#include "engine/graphics/opengl/cBuffer_opengl.h"
#include "engine/graphics/opengl/cVertexArray_opengl.h"

namespace df::opengl
{
	struct sRenderAsset_opengl
	{
		DF_DisableCopyAndMove( sRenderAsset_opengl );

		explicit sRenderAsset_opengl();
		virtual ~sRenderAsset_opengl() = default;

		cVertexArray_opengl vertex_array;
		cBuffer_opengl      vertex_buffer;
		cBuffer_opengl      index_buffer;
	};
}
