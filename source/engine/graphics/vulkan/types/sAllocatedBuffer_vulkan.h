#pragma once

#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

namespace df::vulkan
{
	struct sAllocatedBuffer_vulkan
	{
		void create( vk::DeviceSize _size, vk::BufferUsageFlags _usage_flags, vma::MemoryUsage _memory_usage );
		void destroy();

		void setData( void const* _data, size_t _data_size, size_t _offset, bool _copy = false );

		vma::UniqueBuffer     buffer;
		vma::UniqueAllocation allocation;
	};
}
