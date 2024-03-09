﻿#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include "engine/misc/Misc.h"

namespace df::vulkan
{
	struct sRenderAsset
	{
		DF_DISABLE_COPY_AND_MOVE( sRenderAsset );

		struct sBuffer
		{
			VkBuffer          buffer;
			VmaAllocation     allocation;
			VmaAllocationInfo allocation_info;
		};

		explicit sRenderAsset();
		virtual ~sRenderAsset();

		sBuffer         vertex_buffer;
		sBuffer         index_buffer;
		VkDeviceAddress vertex_buffer_address;
	};
}
