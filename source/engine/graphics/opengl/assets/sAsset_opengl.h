#pragma once

#include "engine/core/utils/Misc.h"
#include "engine/graphics/opengl/buffers/cBuffer_opengl.h"
#include "engine/graphics/opengl/cVertexArray_opengl.h"

namespace df::opengl
{
	struct sAsset_opengl
	{
		DF_DeleteCopyAndMove( sAsset_opengl );

		explicit sAsset_opengl();
		virtual ~sAsset_opengl() = default;

		cVertexArray_opengl m_vertex_array;
		cBuffer_opengl      m_vertex_buffer;
		cBuffer_opengl      m_index_buffer;
	};
}
