#include "sRenderAsset.h"

#include <glad/glad.h>
#include <tracy/Tracy.hpp>

namespace df::vulkan
{
	sRenderAsset::sRenderAsset()
	{
		ZoneScoped;

		glGenVertexArrays( 1, &vertex_array );
		glGenBuffers( 1, &vertex_buffer );
		glGenBuffers( 1, &element_buffer );
	}

	sRenderAsset::~sRenderAsset()
	{
		ZoneScoped;

		glDeleteBuffers( 1, &element_buffer );
		glDeleteBuffers( 1, &vertex_buffer );
		glDeleteVertexArrays( 1, &vertex_array );
	}
}
