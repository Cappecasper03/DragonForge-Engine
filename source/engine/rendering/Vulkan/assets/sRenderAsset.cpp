#include "sRenderAsset.h"

#include <tracy/Tracy.hpp>

#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/Vulkan/cRenderer.h"

namespace df::vulkan
{
	sRenderAsset::sRenderAsset()
		: vertex_buffer( nullptr )
		, vertex_buffer_memory( nullptr )
	{
		ZoneScoped;
	}

	sRenderAsset::~sRenderAsset()
	{
		ZoneScoped;

		const cRenderer* renderer = reinterpret_cast< cRenderer* >( cRendererSingleton::getRenderInstance() );

		vkDestroyBuffer( renderer->logical_device, vertex_buffer, nullptr );
		vkFreeMemory( renderer->logical_device, vertex_buffer_memory, nullptr );
	}
}
