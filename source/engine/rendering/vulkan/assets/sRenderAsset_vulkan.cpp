#include "sRenderAsset_vulkan.h"

#include "engine/profiling/ProfilingMacros.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/types/Helper_vulkan.h"

namespace df::vulkan
{
	sRenderAsset_vulkan::~sRenderAsset_vulkan()
	{
		DF_ProfilingScopeCpu;

		if( reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() )->getLogicalDevice().waitIdle() != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for device idle" );
	}
}
