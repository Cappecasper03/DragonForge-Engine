#include "sRenderAsset_vulkan.h"

#include <tracy/Tracy.hpp>

#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/misc/Helper_vulkan.h"

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

		helper::util::destroyBuffer( index_buffer );
		helper::util::destroyBuffer( vertex_buffer );
	}
}
