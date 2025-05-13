#include "sRenderAsset_opengl.h"

namespace df::opengl
{
	sRenderAsset_opengl::sRenderAsset_opengl()
		: m_vertex_buffer( cBuffer_opengl::kVertex )
		, m_index_buffer( cBuffer_opengl::kIndex )
	{}
}
