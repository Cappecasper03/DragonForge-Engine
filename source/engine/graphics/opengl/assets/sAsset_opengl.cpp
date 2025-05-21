#include "sAsset_opengl.h"

namespace df::opengl
{
	sAsset_opengl::sAsset_opengl()
		: m_vertex_buffer( cBuffer_opengl::kVertex )
		, m_index_buffer( cBuffer_opengl::kIndex )
	{}
}
