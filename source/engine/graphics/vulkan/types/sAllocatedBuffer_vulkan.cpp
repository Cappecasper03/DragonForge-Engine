#include "sAllocatedBuffer_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	void sAllocatedBuffer_vulkan::create( const vk::DeviceSize _size, const vk::BufferUsageFlags _usage_flags, const vma::MemoryUsage _memory_usage )
	{
		DF_ProfilingScopeCpu;

		const vk::BufferCreateInfo buffer_create_info( vk::BufferCreateFlags(), _size, _usage_flags );

		const vma::AllocationCreateInfo allocation_create_info( vma::AllocationCreateFlagBits::eMapped, _memory_usage );

		const vma::Allocator                                  memory_allocator = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() )->getMemoryAllocator();
		std::pair< vma::UniqueBuffer, vma::UniqueAllocation > value            = memory_allocator.createBufferUnique( buffer_create_info, allocation_create_info ).value;
		buffer.swap( value.first );
		allocation.swap( value.second );
	}

	void sAllocatedBuffer_vulkan::setData( void const* _data, const size_t _data_size, const size_t _offset, const bool _copy )
	{
		DF_ProfilingScopeCpu;

		const vma::Allocator memory_allocator = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() )->getMemoryAllocator();

		void* data_dst        = memory_allocator.mapMemory( allocation.get() ).value;
		char* data_dst_offset = static_cast< char* >( data_dst ) + _offset;

		if( _copy )
			std::memcpy( data_dst_offset, _data, _data_size );
		else
			std::memmove( data_dst_offset, _data, _data_size );

		memory_allocator.unmapMemory( allocation.get() );
	}

	void sAllocatedBuffer_vulkan::destroy()
	{
		DF_ProfilingScopeCpu;

		const vma::Allocator memory_allocator = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() )->getMemoryAllocator();

		memory_allocator.destroyBuffer( buffer.get(), allocation.get() );
		buffer.release();
		allocation.release();
	}
}
