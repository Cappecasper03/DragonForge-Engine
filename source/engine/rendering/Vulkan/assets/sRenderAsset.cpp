#include "sRenderAsset.h"

#include <tracy/Tracy.hpp>

#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/Vulkan/cRenderer.h"

namespace df::vulkan
{
	sRenderAsset::sRenderAsset()
		: vertex_buffer( nullptr )
		, allocation( nullptr )
		, allocation_info{}
	{
		ZoneScoped;
	}

	sRenderAsset::~sRenderAsset()
	{
		ZoneScoped;

		const cRenderer* renderer = reinterpret_cast< cRenderer* >( cRendererSingleton::getRenderInstance() );

		vmaDestroyBuffer( renderer->memory_allocator, vertex_buffer, allocation );
	}
}
