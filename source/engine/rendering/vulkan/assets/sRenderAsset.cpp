#include "sRenderAsset.h"

#include <tracy/Tracy.hpp>

#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/vulkan/cRenderer.h"

namespace df::vulkan
{
	sRenderAsset::sRenderAsset()
		: vertex_buffer{}
		, index_buffer{}
		, vertex_buffer_address( 0 )
	{
		ZoneScoped;
	}

	sRenderAsset::~sRenderAsset()
	{
		ZoneScoped;

		const cRenderer* renderer = reinterpret_cast< cRenderer* >( cRendererSingleton::getRenderInstance() );

		vmaDestroyBuffer( renderer->memory_allocator, index_buffer.buffer, index_buffer.allocation );
		vmaDestroyBuffer( renderer->memory_allocator, vertex_buffer.buffer, vertex_buffer.allocation );
	}
}
