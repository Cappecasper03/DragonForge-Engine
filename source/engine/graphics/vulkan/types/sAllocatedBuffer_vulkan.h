#pragma once

#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

namespace df::vulkan
{
	struct sAllocatedBuffer_vulkan
	{
		vma::UniqueBuffer     buffer;
		vma::UniqueAllocation allocation;
	};
}
