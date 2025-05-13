#pragma once

#include "engine/core/utils/Misc.h"
#include "engine/graphics/opengl/cBuffer_opengl.h"
#include "engine/graphics/opengl/cVertexArray_opengl.h"

namespace df::opengl
{
	struct sRenderAsset_opengl
	{
		DF_DisableCopyAndMove( sRenderAsset_opengl );

		explicit sRenderAsset_opengl();
		virtual ~sRenderAsset_opengl() = default;

		cVertexArray_opengl m_vertex_array;
		cBuffer_opengl      m_vertex_buffer;
		cBuffer_opengl      m_index_buffer;
	};
}
