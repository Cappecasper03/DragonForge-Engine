#pragma once

#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

namespace df::vulkan
{
	struct sAllocatedImage_vulkan
	{
		void create( const vk::ImageCreateInfo& _image_info, const vma::AllocationCreateInfo& _allocation_info );
		void create( const vk::ImageViewCreateInfo& _view_info );

		void destroy();

		vma::UniqueImage      image{};
		vk::UniqueImageView   image_view{};
		vma::UniqueAllocation allocation{};
		vk::Extent3D          extent{};
		vk::Format            format;
	};
}
