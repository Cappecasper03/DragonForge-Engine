#include "sAllocatedImage_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	void sAllocatedImage_vulkan::create( const vk::ImageCreateInfo& _image_info, const vma::AllocationCreateInfo& _allocation_info )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		std::pair< vma::UniqueImage, vma::UniqueAllocation > value = graphics_api->getMemoryAllocator().createImageUnique( _image_info, _allocation_info ).value;
		image.swap( value.first );
		allocation.swap( value.second );
	}

	void sAllocatedImage_vulkan::create( const vk::ImageViewCreateInfo& _view_info )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		image_view = graphics_api->getLogicalDevice().createImageViewUnique( _view_info ).value;
	}

	void sAllocatedImage_vulkan::destroy()
	{
		DF_ProfilingScopeCpu;

		const vma::Allocator memory_allocator = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() )->getMemoryAllocator();

		image_view.reset();
		memory_allocator.destroyImage( image.get(), allocation.get() );
		image.release();
		allocation.release();
	}
}