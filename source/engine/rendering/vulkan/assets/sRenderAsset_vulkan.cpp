#include "sRenderAsset_vulkan.h"

#include <tracy/Tracy.hpp>

#include "engine/rendering/cRenderer.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/misc/Helper_vulkan.h"

namespace df::vulkan
{
	sRenderAsset_vulkan::sRenderAsset_vulkan()
		: vertex_buffer{}
		, fragment_buffer{}
		, index_buffer{}
	{
		ZoneScoped;
	}

	sRenderAsset_vulkan::~sRenderAsset_vulkan()
	{
		ZoneScoped;

		vkDeviceWaitIdle( reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() )->logical_device );

		helper::util::destroyBuffer( index_buffer );
		helper::util::destroyBuffer( fragment_buffer );
		helper::util::destroyBuffer( vertex_buffer );
	}
}
