#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include "engine/misc/Misc.h"

namespace df::vulkan
{
	struct sRenderAsset_vulkan
	{
		DF_DISABLE_COPY_AND_MOVE( sRenderAsset_vulkan );

		struct sBuffer
		{
			VkBuffer          buffer;
			VmaAllocation     allocation;
			VmaAllocationInfo allocation_info;
		};

		explicit sRenderAsset_vulkan();
		virtual ~sRenderAsset_vulkan();

		sBuffer         vertex_buffer;
		sBuffer         index_buffer;
		VkDeviceAddress vertex_buffer_address;
	};
}
