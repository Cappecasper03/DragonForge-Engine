#pragma once

#include <vulkan/vulkan_core.h>

#include "engine/misc/Misc.h"
#include "engine/rendering/vulkan/misc/Types_vulkan.h"

namespace df::vulkan
{
	struct sRenderAsset_vulkan
	{
		DF_DISABLE_COPY_AND_MOVE( sRenderAsset_vulkan );

		explicit sRenderAsset_vulkan();
		virtual ~sRenderAsset_vulkan();

		sAllocatedBuffer vertex_buffer;
		sAllocatedBuffer index_buffer;
		VkDeviceAddress  vertex_buffer_address;
	};
}
