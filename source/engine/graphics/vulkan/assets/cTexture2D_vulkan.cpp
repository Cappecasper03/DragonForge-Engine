#include "cTexture2D_vulkan.h"

#include <fmt/format.h>

#include "engine/core/Log.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	cTexture2D_vulkan::~cTexture2D_vulkan()
	{
		DF_ProfilingScopeCpu;

		if( reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() )->getLogicalDevice().waitIdle() != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for device idle" );
	}

	void cTexture2D_vulkan::uploadData( const void* _data, sTextureFormat::eFormat _format, unsigned _mip_level, bool _generate_mipmap )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );

		const uint32_t          data_size = m_description.size.width() * m_description.size.height() * 4;
		sAllocatedBuffer_vulkan buffer    = helper::util::createBuffer( data_size, vk::BufferUsageFlagBits::eTransferSrc, vma::MemoryUsage::eCpuToGpu );

		void* data_dst = renderer->getMemoryAllocator().mapMemory( buffer.allocation.get() ).value;
		std::memcpy( data_dst, _data, data_size );
		renderer->getMemoryAllocator().unmapMemory( buffer.allocation.get() );

		renderer->immediateSubmit(
			[ & ]( const vk::CommandBuffer _command_buffer )
			{
				const vk::Extent3D extent( m_description.size.width(), m_description.size.height(), 1 );

				helper::util::transitionImage( _command_buffer, m_texture.image.get(), vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );

				const vk::BufferImageCopy region( 0, 0, 0, vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ), vk::Offset3D(), extent );

				_command_buffer.copyBufferToImage( buffer.buffer.get(), m_texture.image.get(), vk::ImageLayout::eTransferDstOptimal, 1, &region );

				// TODO: Generate mipmaps

				helper::util::transitionImage( _command_buffer, m_texture.image.get(), vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );
			} );
	}

	void cTexture2D_vulkan::initialize( const sDescription& _description )
	{
		DF_ProfilingScopeCpu;

		cTexture2D::initialize( _description );

		const cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );

		const vk::Format   format = sTextureFormat::toVulkan( m_description.format );
		const vk::Extent3D extent( m_description.size.width(), m_description.size.height(), 1 );

		sAllocatedImage_vulkan image{
			.extent = extent,
			.format = format,
		};

		if( m_description.mip_levels == 1 )
			m_description.mip_levels = 1 + static_cast< unsigned >( std::floor( std::log2( std::max( m_description.size.width(), m_description.size.height() ) ) ) );

		const vk::ImageCreateInfo
			image_create_info( vk::ImageCreateFlags(), vk::ImageType::e2D, format, extent, m_description.mip_levels, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal );

		constexpr vma::AllocationCreateInfo allocation_create_info( vma::AllocationCreateFlags(), vma::MemoryUsage::eGpuOnly, vk::MemoryPropertyFlagBits::eDeviceLocal );

		std::pair< vma::UniqueImage, vma::UniqueAllocation > value = renderer->getMemoryAllocator().createImageUnique( image_create_info, allocation_create_info ).value;
		image.image.swap( value.first );
		image.allocation.swap( value.second );

		vk::ImageAspectFlags aspect_flags = vk::ImageAspectFlagBits::eColor;
		if( format == vk::Format::eD24UnormS8Uint )
			aspect_flags = vk::ImageAspectFlagBits::eDepth;

		const vk::ImageViewCreateInfo image_view_create_info( vk::ImageViewCreateFlags(),
		                                                      image.image.get(),
		                                                      vk::ImageViewType::e2D,
		                                                      format,
		                                                      vk::ComponentMapping(),
		                                                      vk::ImageSubresourceRange( aspect_flags, 0, 1, 0, image_create_info.mipLevels ) );

		image.image_view = renderer->getLogicalDevice().createImageViewUnique( image_view_create_info ).value;
	}
}
