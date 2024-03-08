#pragma once

#include <vulkan/vulkan_core.h>

#include "engine/misc/Misc.h"

namespace df::vulkan
{
	struct sRenderAsset
	{
		DF_DISABLE_COPY_AND_MOVE( sRenderAsset );

		explicit sRenderAsset();
		virtual ~sRenderAsset();

		VkBuffer       vertex_buffer;
		VkDeviceMemory vertex_buffer_memory;
	};
}
