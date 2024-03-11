#include "sRenderAsset_vulkan.h"

#include <tracy/Tracy.hpp>

#include "engine/rendering/cRenderer.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"

namespace df::vulkan
{
	sRenderAsset_vulkan::sRenderAsset_vulkan()
		: vertex_buffer{}
		, index_buffer{}
		, vertex_buffer_address( 0 )
	{
		ZoneScoped;
	}

	sRenderAsset_vulkan::~sRenderAsset_vulkan()
	{
		ZoneScoped;

		const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

		vmaDestroyBuffer( renderer->memory_allocator, index_buffer.buffer, index_buffer.allocation );
		vmaDestroyBuffer( renderer->memory_allocator, vertex_buffer.buffer, vertex_buffer.allocation );
	}
}
