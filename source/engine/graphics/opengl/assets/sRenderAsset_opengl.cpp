#include "sRenderAsset_opengl.h"

namespace df::opengl
{
	sRenderAsset_opengl::sRenderAsset_opengl()
		: vertex_buffer( cBuffer_opengl::kVertex )
		, index_buffer( cBuffer_opengl::kIndex )
	{}
}
