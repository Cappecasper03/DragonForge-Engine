#pragma once

#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

namespace df::vulkan
{
	struct sAllocatedImage_vulkan
	{
		vma::UniqueImage      image{};
		vk::UniqueImageView   image_view{};
		vma::UniqueAllocation allocation{};
		vk::Extent3D          extent{};
		vk::Format            format{};
	};
}
