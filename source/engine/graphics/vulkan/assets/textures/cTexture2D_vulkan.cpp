#include "cTexture2D_vulkan.h"

#include <fmt/format.h>

#include "engine/core/Log.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/graphics/vulkan/types/Helper_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	cTexture2D_vulkan::~cTexture2D_vulkan()
	{
		DF_ProfilingScopeCpu;

		if( reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() )->getLogicalDevice().waitIdle() != vk::Result::eSuccess )
			DF_LogError( "Failed to wait for device idle" );
	}

	void cTexture2D_vulkan::clear( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		const vk::ClearColorValue       clear_color_value( _color.r, _color.g, _color.b, _color.a );
		const vk::ImageSubresourceRange subresource_range( vk::ImageAspectFlagBits::eColor, 0, m_description.mip_levels, 0, 1 );

		graphics_api->immediateSubmit(
			[ & ]( const vk::CommandBuffer _command_buffer )
			{
				helper::util::transitionImage( _command_buffer, m_texture.image.get(), vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );

				_command_buffer.clearColorImage( m_texture.image.get(), vk::ImageLayout::eTransferDstOptimal, &clear_color_value, 1, &subresource_range );

				helper::util::transitionImage( _command_buffer, m_texture.image.get(), vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );
			} );
	}

	void cTexture2D_vulkan::uploadData( const void* _data, const sTextureFormat::eFormat _format, unsigned _mip_level, bool _generate_mipmap )
	{
		DF_ProfilingScopeCpu;

		const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		const uint32_t          data_size = m_description.size.width() * m_description.size.height() * sTextureFormat::bytesPerPixel( m_description.format );
		sAllocatedBuffer_vulkan buffer    = helper::util::createBuffer( data_size, vk::BufferUsageFlagBits::eTransferSrc, vma::MemoryUsage::eCpuToGpu );

		void* data_dst = graphics_api->getMemoryAllocator().mapMemory( buffer.allocation.get() ).value;
		std::memcpy( data_dst, _data, data_size );
		graphics_api->getMemoryAllocator().unmapMemory( buffer.allocation.get() );

		graphics_api->immediateSubmit(
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

		const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

		const vk::Format   format = sTextureFormat::toVulkan( m_description.format );
		const vk::Extent3D extent( m_description.size.width(), m_description.size.height(), 1 );

		m_texture = {
			.extent = extent,
			.format = format,
		};

		if( m_description.mip_levels == 0 )
			m_description.mip_levels = 1 + static_cast< unsigned >( std::floor( std::log2( std::max( m_description.size.width(), m_description.size.height() ) ) ) );

		const vk::ImageCreateInfo image_create_info( vk::ImageCreateFlags(),
		                                             vk::ImageType::e2D,
		                                             format,
		                                             extent,
		                                             m_description.mip_levels,
		                                             1,
		                                             vk::SampleCountFlagBits::e1,
		                                             vk::ImageTiling::eOptimal,
		                                             sTextureUsage::toVulkan( m_description.usage ) );

		constexpr vma::AllocationCreateInfo allocation_create_info( vma::AllocationCreateFlags(), vma::MemoryUsage::eGpuOnly, vk::MemoryPropertyFlagBits::eDeviceLocal );

		std::pair< vma::UniqueImage, vma::UniqueAllocation > value = graphics_api->getMemoryAllocator().createImageUnique( image_create_info, allocation_create_info ).value;
		m_texture.image.swap( value.first );
		m_texture.allocation.swap( value.second );

		const vk::ImageViewCreateInfo image_view_create_info( vk::ImageViewCreateFlags(),
		                                                      m_texture.image.get(),
		                                                      vk::ImageViewType::e2D,
		                                                      format,
		                                                      vk::ComponentMapping(),
		                                                      vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, image_create_info.mipLevels, 0, 1 ) );

		m_texture.image_view = graphics_api->getLogicalDevice().createImageViewUnique( image_view_create_info ).value;
	}
}
