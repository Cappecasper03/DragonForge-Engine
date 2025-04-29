#include "sRenderAsset_opengl.h"

namespace df::opengl
{
	sRenderAsset_opengl::sRenderAsset_opengl()
		: vertex_buffer( cBuffer_opengl::kVertexBuffer )
		, index_buffer( cBuffer_opengl::kIndexBuffer )
	{}
}
