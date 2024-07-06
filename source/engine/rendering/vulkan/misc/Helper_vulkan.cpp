#include "Helper_vulkan.h"

#include <fmt/format.h>
#include <tracy/Tracy.hpp>
#include <vector>

#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"

namespace df::vulkan::helper
{
	namespace init
	{
		vk::CommandPoolCreateInfo commandPoolCreateInfo( const uint32_t _queue_family_index, const vk::CommandPoolCreateFlags _flags )
		{
			ZoneScoped;

			const vk::CommandPoolCreateInfo create_info( _flags, _queue_family_index );
			return create_info;
		}

		vk::CommandBufferAllocateInfo commandBufferAllocateInfo( const vk::CommandPool _command_pool, const uint32_t _count )
		{
			ZoneScoped;

			const vk::CommandBufferAllocateInfo allocate_info( _command_pool, vk::CommandBufferLevel::ePrimary, _count );
			return allocate_info;
		}

		vk::CommandBufferBeginInfo commandBufferBeginInfo( const vk::CommandBufferUsageFlags _flags )
		{
			ZoneScoped;

			const vk::CommandBufferBeginInfo begin_info( _flags );
			return begin_info;
		}

		vk::FenceCreateInfo fenceCreateInfo( const vk::FenceCreateFlags _flags )
		{
			ZoneScoped;

			const vk::FenceCreateInfo create_info( _flags );
			return create_info;
		}
		vk::SemaphoreCreateInfo semaphoreCreateInfo()
		{
			ZoneScoped;

			return vk::SemaphoreCreateInfo();
		}

		vk::SemaphoreSubmitInfo semaphoreSubmitInfo( const vk::PipelineStageFlags2 _stage_mask, const vk::Semaphore _semaphore )
		{
			ZoneScoped;

			const vk::SemaphoreSubmitInfo submit_info( _semaphore, static_cast< uint32_t >( 1 ), _stage_mask, static_cast< uint32_t >( 0 ) );
			return submit_info;
		}

		vk::CommandBufferSubmitInfo commandBufferSubmitInfo( const vk::CommandBuffer _command_buffer )
		{
			ZoneScoped;

			const vk::CommandBufferSubmitInfo submit_info( _command_buffer, static_cast< uint32_t >( 0 ) );
			return submit_info;
		}

		vk::SubmitInfo2 submitInfo( const vk::CommandBufferSubmitInfo* _command_buffer, const vk::SemaphoreSubmitInfo* _signal_semaphore_info, const vk::SemaphoreSubmitInfo* _wait_semaphore_info )
		{
			ZoneScoped;

			const uint32_t wait_count    = _wait_semaphore_info ? 1 : 0;
			const uint32_t command_count = _command_buffer ? 1 : 0;
			const uint32_t signal_count  = _signal_semaphore_info ? 1 : 0;

			const vk::SubmitInfo2 submit_info( {}, wait_count, _wait_semaphore_info, command_count, _command_buffer, signal_count, _signal_semaphore_info );
			return submit_info;
		}

		vk::PresentInfoKHR presentInfo( const vk::Semaphore _semaphore, const vk::SwapchainKHR _swapchain, const uint32_t _swap_chain_index )
		{
			ZoneScoped;

			const vk::PresentInfoKHR present_info( 1, &_semaphore, 1, &_swapchain, &_swap_chain_index );
			return present_info;
		}

		vk::RenderingAttachmentInfo attachmentInfo( const vk::ImageView _view, const vk::ClearValue* _clear, const vk::ImageLayout _layout )
		{
			ZoneScoped;

			const vk::RenderingAttachmentInfo attachment_info( _view,
			                                                   _layout,
			                                                   vk::ResolveModeFlagBits::eNone,
			                                                   nullptr,
			                                                   vk::ImageLayout::eUndefined,
			                                                   _clear ? vk::AttachmentLoadOp::eClear : vk::AttachmentLoadOp::eLoad,
			                                                   vk::AttachmentStoreOp::eStore,
			                                                   _clear ? *_clear : vk::ClearValue{} );
			return attachment_info;
		}

		vk::RenderingAttachmentInfo depthAttachmentInfo( const vk::ImageView _view, const vk::ImageLayout _layout )
		{
			ZoneScoped;

			const vk::RenderingAttachmentInfo attachment_info( _view,
			                                                   _layout,
			                                                   vk::ResolveModeFlagBits::eNone,
			                                                   nullptr,
			                                                   vk::ImageLayout::eUndefined,
			                                                   vk::AttachmentLoadOp::eClear,
			                                                   vk::AttachmentStoreOp::eStore,
			                                                   vk::ClearValue( vk::ClearDepthStencilValue( 0 ) ) );
			return attachment_info;
		}

		vk::ImageCreateInfo imageCreateInfo( const vk::Format _format, const vk::ImageUsageFlags _usage_flags, const vk::Extent3D _extent )
		{
			ZoneScoped;

			const vk::ImageCreateInfo create_info( {}, vk::ImageType::e2D, _format, _extent, 1, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal, _usage_flags );
			return create_info;
		}

		vk::ImageViewCreateInfo imageViewCreateInfo( const vk::Format _format, const vk::Image _image, const vk::ImageAspectFlags _aspect_flags )
		{
			ZoneScoped;

			const vk::ImageViewCreateInfo info( {}, _image, vk::ImageViewType::e2D, _format, {}, vk::ImageSubresourceRange( _aspect_flags, 0, 1, 0, 1 ) );
			return info;
		}

		vk::ImageSubresourceRange imageSubresourceRange( const vk::ImageAspectFlags _aspect_mask )
		{
			ZoneScoped;

			const vk::ImageSubresourceRange subresource_range( _aspect_mask, 0, vk::RemainingMipLevels, 0, vk::RemainingArrayLayers );
			return subresource_range;
		}

		vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfo( const vk::ShaderStageFlagBits _stage, const vk::ShaderModule _module )
		{
			ZoneScoped;

			const vk::PipelineShaderStageCreateInfo info( {}, _stage, _module, "main" );
			return info;
		}

		vk::RenderingInfo renderingInfo( const vk::Extent2D _extent, const vk::RenderingAttachmentInfo* _color_attachment, const vk::RenderingAttachmentInfo* _depth_attachment )
		{
			ZoneScoped;

			const vk::RenderingInfo info( {}, vk::Rect2D( {}, _extent ), 1, 0, 1, _color_attachment, _depth_attachment );
			return info;
		}
	}

	namespace util
	{
		void transitionImage( const vk::CommandBuffer _command_buffer, const vk::Image _image, const vk::ImageLayout _current_layout, const vk::ImageLayout _new_layout )
		{
			ZoneScoped;

			const vk::ImageMemoryBarrier2 memory_barrier(
				vk::PipelineStageFlagBits2::eAllCommands,
				vk::AccessFlagBits2::eMemoryWrite,
				vk::PipelineStageFlagBits2::eAllCommands,
				vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
				_current_layout,
				_new_layout,
				0,
				0,
				_image,
				init::imageSubresourceRange( _new_layout == vk::ImageLayout::eDepthAttachmentOptimal ? vk::ImageAspectFlagBits::eDepth : vk::ImageAspectFlagBits::eColor ) );

			const vk::DependencyInfo info( {}, 0, nullptr, 0, nullptr, 1, &memory_barrier );
			_command_buffer.pipelineBarrier2( info );
		}

		void copyImageToImage( const vk::CommandBuffer _command_buffer, const vk::Image _source, const vk::Image _destination, const vk::Extent2D _source_size, const vk::Extent2D _destination_size )
		{
			ZoneScoped;

			const vk::ImageBlit2 blit_region(
				vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ),
				std::array< vk::Offset3D, 2 >( { {}, vk::Offset3D( static_cast< int32_t >( _source_size.width ), static_cast< int32_t >( _source_size.height ), 1 ) } ),
				vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ),
				std::array< vk::Offset3D, 2 >( { {}, vk::Offset3D( static_cast< int32_t >( _destination_size.width ), static_cast< int32_t >( _destination_size.height ), 1 ) } ) );

			const vk::BlitImageInfo2 blit_info( _source, vk::ImageLayout::eTransferSrcOptimal, _destination, vk::ImageLayout::eTransferDstOptimal, 1, &blit_region, vk::Filter::eLinear );
			_command_buffer.blitImage2( blit_info );
		}

		vk::ShaderModule createShaderModule( const std::string& _name )
		{
			ZoneScoped;

			const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

			std::vector< char > shader;
			vk::ShaderModule    module = nullptr;

			std::fstream shader_file = filesystem::open( fmt::format( "binaries/shaders/vulkan/{}.spv", _name ), std::ios::in | std::ios::ate | std::ios::binary );
			if( !shader_file.is_open() )
			{
				DF_LOG_ERROR( fmt::format( "Failed to load shader: {}", _name ) );
				return module;
			}

			shader.resize( shader_file.tellg() );
			shader_file.seekg( 0 );
			shader_file.read( shader.data(), static_cast< long long >( shader.size() ) );

			const vk::ShaderModuleCreateInfo create_info( {}, shader.size(), reinterpret_cast< const uint32_t* >( shader.data() ) );

			module = renderer->getLogicalDevice()->createShaderModule( create_info ).value;
			DF_LOG_MESSAGE( fmt::format( "Successfully loaded shader and created shader module: {}", _name ) );
			return module;
		}

		void createBuffer( const vk::DeviceSize _size, const vk::BufferUsageFlags _usage_flags, const VmaMemoryUsage _memory_usage, sAllocatedBuffer& _buffer )
		{
			ZoneScoped;

			createBuffer( _size, _usage_flags, _memory_usage, _buffer, reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() )->memory_allocator );
		}

		void createBuffer( const vk::DeviceSize _size, const vk::BufferUsageFlags _usage_flags, const VmaMemoryUsage _memory_usage, sAllocatedBuffer& _buffer, const VmaAllocator _memory_allocator )
		{
			ZoneScoped;

			const vk::BufferCreateInfo buffer_create_info( {}, _size, _usage_flags );

			const VmaAllocationCreateInfo allocation_create_info{
				.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
				.usage = _memory_usage,
			};

			vmaCreateBuffer( _memory_allocator,
			                 reinterpret_cast< const VkBufferCreateInfo* >( &buffer_create_info ),
			                 &allocation_create_info,
			                 reinterpret_cast< VkBuffer* >( &_buffer.buffer ),
			                 &_buffer.allocation,
			                 &_buffer.allocation_info );
		}

		sAllocatedBuffer createBuffer( const vk::DeviceSize _size, const vk::BufferUsageFlags _usage_flags, const VmaMemoryUsage _memory_usage )
		{
			ZoneScoped;

			sAllocatedBuffer buffer{};
			createBuffer( _size, _usage_flags, _memory_usage, buffer );
			return buffer;
		}

		sAllocatedBuffer createBuffer( const vk::DeviceSize _size, const vk::BufferUsageFlags _usage_flags, const VmaMemoryUsage _memory_usage, const VmaAllocator _memory_allocator )
		{
			ZoneScoped;

			sAllocatedBuffer buffer{};
			createBuffer( _size, _usage_flags, _memory_usage, buffer, _memory_allocator );
			return buffer;
		}

		void destroyBuffer( const sAllocatedBuffer& _buffer )
		{
			ZoneScoped;

			const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

			vmaDestroyBuffer( renderer->memory_allocator, _buffer.buffer.get(), _buffer.allocation );
		}

		void destroyBuffer( const sAllocatedBuffer& _buffer, const VmaAllocator _memory_allocator )
		{
			ZoneScoped;

			vmaDestroyBuffer( _memory_allocator, _buffer.buffer.get(), _buffer.allocation );
		}

		sAllocatedImage createImage( const vk::Extent3D _size, const vk::Format _format, const vk::ImageUsageFlags _usage, const bool _mipmapped, const unsigned _mipmaps )
		{
			ZoneScoped;

			const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

			sAllocatedImage image{
				.extent = _size,
				.format = _format,
			};

			vk::ImageCreateInfo image_create_info = init::imageCreateInfo( _format, _usage, _size );
			if( _mipmapped )
			{
				if( _mipmaps >= 1 )
					image_create_info.mipLevels = _mipmaps;
				else
					image_create_info.mipLevels = static_cast< uint32_t >( std::floor( std::log2( std::max( _size.width, _size.height ) ) ) ) + 1;
			}

			constexpr VmaAllocationCreateInfo allocation_create_info{
				.usage         = VMA_MEMORY_USAGE_GPU_ONLY,
				.requiredFlags = static_cast< VkMemoryPropertyFlags >( vk::MemoryPropertyFlagBits::eDeviceLocal ),
			};

			vmaCreateImage( renderer->memory_allocator,
			                reinterpret_cast< const VkImageCreateInfo* >( &image_create_info ),
			                &allocation_create_info,
			                reinterpret_cast< VkImage* >( &image.image.get() ),
			                &image.allocation,
			                nullptr );

			vk::ImageAspectFlags aspect_flags = vk::ImageAspectFlagBits::eColor;
			if( _format == vk::Format::eD32Sfloat )
				aspect_flags = vk::ImageAspectFlagBits::eDepth;

			vk::ImageViewCreateInfo image_view_create_info     = init::imageViewCreateInfo( _format, image.image.get(), aspect_flags );
			image_view_create_info.subresourceRange.levelCount = image_create_info.mipLevels;

			image.image_view = renderer->getLogicalDevice()->createImageViewUnique( image_view_create_info ).value;
			return image;
		}

		sAllocatedImage createImage( const void* _data, const vk::Extent3D _size, const vk::Format _format, const vk::ImageUsageFlags _usage, const bool _mipmapped, const unsigned _mipmaps )
		{
			ZoneScoped;

			const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

			const uint32_t         data_size = _size.depth * _size.width * _size.height * 4;
			const sAllocatedBuffer buffer    = createBuffer( data_size, vk::BufferUsageFlagBits::eTransferSrc, VMA_MEMORY_USAGE_CPU_TO_GPU );

			std::memcpy( buffer.allocation_info.pMappedData, _data, data_size );

			sAllocatedImage image = createImage( _size, _format, _usage | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc, _mipmapped, _mipmaps );

			renderer->immediateSubmit(
				[ & ]( const vk::CommandBuffer _command_buffer )
				{
					transitionImage( _command_buffer, image.image.get(), vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );

					const vk::BufferImageCopy region( 0, 0, 0, vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ), {}, _size );

					_command_buffer.copyBufferToImage( buffer.buffer.get(), image.image.get(), vk::ImageLayout::eTransferDstOptimal, 1, &region );

					transitionImage( _command_buffer, image.image.get(), vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );
				} );

			destroyBuffer( buffer );

			return image;
		}

		void destroyImage( const sAllocatedImage& _image )
		{
			ZoneScoped;

			const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

			vmaDestroyImage( renderer->memory_allocator, _image.image.get(), _image.allocation );
		}
	}
}
