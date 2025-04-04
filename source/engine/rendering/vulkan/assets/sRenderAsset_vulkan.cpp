#include "sRenderAsset_vulkan.h"

#include "engine/profiling/ProfilingMacros.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/types/Helper_vulkan.h"

namespace df::vulkan
{
	sRenderAsset_vulkan::~sRenderAsset_vulkan()
	{
		DF_ProfilingScopeCPU;

		if( reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() )->getLogicalDevice().waitIdle() != vk::Result::eSuccess )
			DF_LOG_ERROR( "Failed to wait for device idle" );
	}
}
