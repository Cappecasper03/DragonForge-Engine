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
		VkCommandPoolCreateInfo commandPoolCreateInfo( const uint32_t _queue_family_index, const VkCommandPoolCreateFlags _flags )
		{
			ZoneScoped;

			const VkCommandPoolCreateInfo info{
				.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				.pNext            = nullptr,
				.flags            = _flags,
				.queueFamilyIndex = _queue_family_index,
			};

			return info;
		}

		VkCommandBufferAllocateInfo commandBufferAllocateInfo( const VkCommandPool _command_pool, const uint32_t _count )
		{
			ZoneScoped;

			const VkCommandBufferAllocateInfo info = {
				.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				.pNext              = nullptr,
				.commandPool        = _command_pool,
				.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = _count,
			};

			return info;
		}

		VkCommandBufferBeginInfo commandBufferBeginInfo( const VkCommandBufferUsageFlags _flags )
		{
			ZoneScoped;

			const VkCommandBufferBeginInfo info{
				.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				.pNext            = nullptr,
				.flags            = _flags,
				.pInheritanceInfo = nullptr,
			};

			return info;
		}

		VkFenceCreateInfo fenceCreateInfo( const VkFenceCreateFlags _flags )
		{
			ZoneScoped;

			const VkFenceCreateInfo info = {
				.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				.pNext = nullptr,
				.flags = _flags,
			};

			return info;
		}
		VkSemaphoreCreateInfo semaphoreCreateInfo( const VkSemaphoreCreateFlags _flags )
		{
			ZoneScoped;

			const VkSemaphoreCreateInfo info = {
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
				.pNext = nullptr,
				.flags = _flags,
			};

			return info;
		}

		VkSemaphoreSubmitInfo semaphoreSubmitInfo( const VkPipelineStageFlags2 _stage_mask, const VkSemaphore _semaphore )
		{
			const VkSemaphoreSubmitInfo submit_info{
				.sType       = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
				.pNext       = nullptr,
				.semaphore   = _semaphore,
				.value       = 1,
				.stageMask   = _stage_mask,
				.deviceIndex = 0,
			};

			return submit_info;
		}

		VkCommandBufferSubmitInfo commandBufferSubmitInfo( const VkCommandBuffer _command_buffer )
		{
			const VkCommandBufferSubmitInfo submit_info{
				.sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
				.pNext         = nullptr,
				.commandBuffer = _command_buffer,
				.deviceMask    = 0,
			};

			return submit_info;
		}

		VkSubmitInfo2 submitInfo( VkCommandBufferSubmitInfo* _command_buffer, VkSemaphoreSubmitInfo* _signal_semaphore_info, VkSemaphoreSubmitInfo* _wait_semaphore_info )
		{
			const uint32_t wait_count    = _wait_semaphore_info ? 1 : 0;
			const uint32_t command_count = _command_buffer ? 1 : 0;
			const uint32_t signal_count  = _signal_semaphore_info ? 1 : 0;

			const VkSubmitInfo2 submit_info{
				.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
				.pNext                    = nullptr,
				.waitSemaphoreInfoCount   = wait_count,
				.pWaitSemaphoreInfos      = _wait_semaphore_info,
				.commandBufferInfoCount   = command_count,
				.pCommandBufferInfos      = _command_buffer,
				.signalSemaphoreInfoCount = signal_count,
				.pSignalSemaphoreInfos    = _signal_semaphore_info,
			};

			return submit_info;
		}

		VkPresentInfoKHR presentInfo()
		{
			ZoneScoped;

			constexpr VkPresentInfoKHR info = {
				.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				.pNext              = nullptr,
				.waitSemaphoreCount = 0,
				.pWaitSemaphores    = nullptr,
				.swapchainCount     = 0,
				.pSwapchains        = nullptr,
				.pImageIndices      = nullptr,
			};

			return info;
		}

		VkRenderingAttachmentInfo attachmentInfo( const VkImageView _view, const VkClearValue* _clear, const VkImageLayout _layout )
		{
			ZoneScoped;

			const VkRenderingAttachmentInfo info{
				.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
				.pNext       = nullptr,
				.imageView   = _view,
				.imageLayout = _layout,
				.loadOp      = _clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
				.storeOp     = VK_ATTACHMENT_STORE_OP_STORE,
				.clearValue  = _clear ? *_clear : VkClearValue{},
			};

			return info;
		}

		VkRenderingAttachmentInfo depthAttachmentInfo( const VkImageView _view, const VkImageLayout _layout )
		{
			ZoneScoped;

			const VkRenderingAttachmentInfo info{
				.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
				.pNext       = nullptr,
				.imageView   = _view,
				.imageLayout = _layout,
				.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp     = VK_ATTACHMENT_STORE_OP_STORE,
				.clearValue  = {
					.depthStencil = {
						.depth = 0,
					},
				},
			};

			return info;
		}

		VkImageCreateInfo imageCreateInfo( const VkFormat _format, const VkImageUsageFlags _usage_flags, const VkExtent3D _extent )
		{
			ZoneScoped;

			const VkImageCreateInfo info{
				.sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext       = nullptr,
				.imageType   = VK_IMAGE_TYPE_2D,
				.format      = _format,
				.extent      = _extent,
				.mipLevels   = 1,
				.arrayLayers = 1,
				.samples     = VK_SAMPLE_COUNT_1_BIT,
				.tiling      = VK_IMAGE_TILING_OPTIMAL,
				.usage       = _usage_flags,
			};

			return info;
		}

		VkImageViewCreateInfo imageViewCreateInfo( const VkFormat _format, const VkImage _image, const VkImageAspectFlags _aspect_flags )
		{
			ZoneScoped;

			const VkImageViewCreateInfo info{
			.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext            = nullptr,
			.image            = _image,
			.viewType         = VK_IMAGE_VIEW_TYPE_2D,
			.format           = _format,
			.subresourceRange = {
				.aspectMask     = _aspect_flags,
				.baseMipLevel   = 0,
				.levelCount     = 1,
				.baseArrayLayer = 0,
				.layerCount     = 1,
			},
		};

			return info;
		}

		VkImageSubresourceRange imageSubresourceRange( const VkImageAspectFlags _aspect_mask )
		{
			ZoneScoped;

			const VkImageSubresourceRange subresource_range{
				.aspectMask     = _aspect_mask,
				.baseMipLevel   = 0,
				.levelCount     = VK_REMAINING_MIP_LEVELS,
				.baseArrayLayer = 0,
				.layerCount     = VK_REMAINING_ARRAY_LAYERS,
			};

			return subresource_range;
		}

		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo( const VkShaderStageFlagBits _stage, const VkShaderModule _module )
		{
			ZoneScoped;

			const VkPipelineShaderStageCreateInfo info{
				.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.pNext  = nullptr,
				.stage  = _stage,
				.module = _module,
				.pName  = "main",
			};

			return info;
		}

		VkRenderingInfo renderingInfo( const VkExtent2D _extent, const VkRenderingAttachmentInfo* _color_attachment, const VkRenderingAttachmentInfo* _depth_attachment )
		{
			ZoneScoped;

			const VkRenderingInfo info{
				.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO,
				.pNext                = nullptr,
				.renderArea           = {
					.offset = {
						.x = 0,
						.y = 0,
					},
					.extent =_extent,
				},
				.layerCount           = 1,
				.viewMask             = 0,
				.colorAttachmentCount = 1,
				.pColorAttachments    = _color_attachment,
				.pDepthAttachment     = _depth_attachment,
			};

			return info;
		}
	}

	namespace util
	{
		void transitionImage( const VkCommandBuffer _command_buffer, const VkImage _image, const VkImageLayout _current_layout, const VkImageLayout _new_layout )
		{
			ZoneScoped;

			VkImageMemoryBarrier2 memory_barrier{
				.sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
				.pNext            = nullptr,
				.srcStageMask     = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
				.srcAccessMask    = VK_ACCESS_2_MEMORY_WRITE_BIT,
				.dstStageMask     = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
				.dstAccessMask    = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT,
				.oldLayout        = _current_layout,
				.newLayout        = _new_layout,
				.image            = _image,
				.subresourceRange = init::imageSubresourceRange( _new_layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT ),
			};

			const VkDependencyInfo info{
				.sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
				.pNext                   = nullptr,
				.imageMemoryBarrierCount = 1,
				.pImageMemoryBarriers    = &memory_barrier,
			};

			vkCmdPipelineBarrier2( _command_buffer, &info );
		}

		void copyImageToImage( const VkCommandBuffer _command_buffer, const VkImage _source, const VkImage _destination, const VkExtent2D _source_size, const VkExtent2D _destination_size )
		{
			ZoneScoped;

			VkImageBlit2 blit_region{
				.sType          = VK_STRUCTURE_TYPE_IMAGE_BLIT_2,
				.pNext          = nullptr,
				.srcSubresource = {
					.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
					.mipLevel       = 0,
					.baseArrayLayer = 0,
					.layerCount     = 1,
				},
				.srcOffsets = {
					{},
					{
						.x = static_cast< int32_t >( _source_size.width ),
						.y = static_cast< int32_t >( _source_size.height ),
						.z = 1,
					}
				},
				.dstSubresource = {
					.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
					.mipLevel       = 0,
					.baseArrayLayer = 0,
					.layerCount     = 1,
				},
				.dstOffsets = {
					{},
					{
						.x = static_cast< int32_t >( _destination_size.width ),
						.y = static_cast< int32_t >( _destination_size.height ),
						.z = 1,
					}
				},
			};

			const VkBlitImageInfo2 blit_info{
				.sType          = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2,
				.pNext          = nullptr,
				.srcImage       = _source,
				.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				.dstImage       = _destination,
				.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				.regionCount    = 1,
				.pRegions       = &blit_region,
				.filter         = VK_FILTER_LINEAR,
			};

			vkCmdBlitImage2( _command_buffer, &blit_info );
		}

		VkShaderModule createShaderModule( const std::string& _name )
		{
			ZoneScoped;

			const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

			std::vector< char > shader;
			VkShaderModule      module = nullptr;

			std::fstream shader_file = filesystem::open( fmt::format( "binaries/shaders/vulkan/{}.spv", _name ), std::ios::in | std::ios::ate | std::ios::binary );
			if( !shader_file.is_open() )
			{
				DF_LOG_ERROR( fmt::format( "Failed to load shader: {}", _name ) );
				return module;
			}

			shader.resize( shader_file.tellg() );
			shader_file.seekg( 0 );
			shader_file.read( shader.data(), static_cast< long long >( shader.size() ) );

			const VkShaderModuleCreateInfo create_info{
				.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				.codeSize = shader.size(),
				.pCode    = reinterpret_cast< const uint32_t* >( shader.data() ),
			};

			if( vkCreateShaderModule( renderer->logical_device, &create_info, nullptr, &module ) != VK_SUCCESS )
				DF_LOG_ERROR( "Failed to create shader module" );

			DF_LOG_MESSAGE( fmt::format( "Successfully loaded shader and created shader module: {}", _name ) );
			return module;
		}

		void createBuffer( const VkDeviceSize _size, const VkBufferUsageFlags _usage_flags, const VmaMemoryUsage _memory_usage, sAllocatedBuffer& _buffer )
		{
			ZoneScoped;

			createBuffer( _size, _usage_flags, _memory_usage, _buffer, reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() )->memory_allocator );
		}

		void createBuffer( const VkDeviceSize _size, const VkBufferUsageFlags _usage_flags, const VmaMemoryUsage _memory_usage, sAllocatedBuffer& _buffer, const VmaAllocator _memory_allocator )
		{
			ZoneScoped;

			const VkBufferCreateInfo buffer_create_info{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.pNext = nullptr,
				.size  = _size,
				.usage = _usage_flags,
			};

			const VmaAllocationCreateInfo allocation_create_info{
				.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
				.usage = _memory_usage,
			};

			vmaCreateBuffer( _memory_allocator, &buffer_create_info, &allocation_create_info, &_buffer.buffer, &_buffer.allocation, &_buffer.allocation_info );
		}

		sAllocatedBuffer createBuffer( const VkDeviceSize _size, const VkBufferUsageFlags _usage_flags, const VmaMemoryUsage _memory_usage )
		{
			ZoneScoped;

			sAllocatedBuffer buffer{};
			createBuffer( _size, _usage_flags, _memory_usage, buffer );
			return buffer;
		}

		sAllocatedBuffer createBuffer( const VkDeviceSize _size, const VkBufferUsageFlags _usage_flags, const VmaMemoryUsage _memory_usage, const VmaAllocator _memory_allocator )
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

			vmaDestroyBuffer( renderer->memory_allocator, _buffer.buffer, _buffer.allocation );
		}

		void destroyBuffer( const sAllocatedBuffer& _buffer, const VmaAllocator _memory_allocator )
		{
			ZoneScoped;

			vmaDestroyBuffer( _memory_allocator, _buffer.buffer, _buffer.allocation );
		}

		sAllocatedImage createImage( const VkExtent3D _size, const VkFormat _format, const VkImageUsageFlags _usage, const bool _mipmapped, const unsigned _mipmaps )
		{
			ZoneScoped;

			const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

			sAllocatedImage image{
				.extent = _size,
				.format = _format,
			};

			VkImageCreateInfo image_create_info = init::imageCreateInfo( _format, _usage, _size );
			if( _mipmapped )
			{
				if( _mipmaps >= 1 )
					image_create_info.mipLevels = _mipmaps;
				else
					image_create_info.mipLevels = static_cast< uint32_t >( std::floor( std::log2( std::max( _size.width, _size.height ) ) ) ) + 1;
			}

			constexpr VmaAllocationCreateInfo allocation_create_info{
				.usage         = VMA_MEMORY_USAGE_GPU_ONLY,
				.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			};

			vmaCreateImage( renderer->memory_allocator, &image_create_info, &allocation_create_info, &image.image, &image.allocation, nullptr );

			VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;
			if( _format == VK_FORMAT_D32_SFLOAT )
				aspect_flags = VK_IMAGE_ASPECT_DEPTH_BIT;

			VkImageViewCreateInfo image_view_create_info       = init::imageViewCreateInfo( _format, image.image, aspect_flags );
			image_view_create_info.subresourceRange.levelCount = image_create_info.mipLevels;

			vkCreateImageView( renderer->logical_device, &image_view_create_info, nullptr, &image.image_view );

			return image;
		}

		sAllocatedImage createImage( const void* _data, const VkExtent3D _size, const VkFormat _format, const VkImageUsageFlags _usage, const bool _mipmapped, const unsigned _mipmaps )
		{
			ZoneScoped;

			const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

			const uint32_t         data_size = _size.depth * _size.width * _size.height * 4;
			const sAllocatedBuffer buffer    = createBuffer( data_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU );

			std::memcpy( buffer.allocation_info.pMappedData, _data, data_size );

			const sAllocatedImage image = createImage( _size, _format, _usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, _mipmapped, _mipmaps );

			renderer->immediateSubmit(
				[ & ]( const VkCommandBuffer _command_buffer )
				{
					transitionImage( _command_buffer, image.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );

					const VkBufferImageCopy region{
						.bufferOffset      = 0,
						.bufferRowLength   = 0,
						.bufferImageHeight = 0,
						.imageSubresource  = {
							.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
							.mipLevel       = 0,
							.baseArrayLayer = 0,
							.layerCount     = 1,
						},
						.imageExtent       = _size,
					};

					vkCmdCopyBufferToImage( _command_buffer, buffer.buffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region );

					transitionImage( _command_buffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
				} );

			destroyBuffer( buffer );

			return image;
		}

		void destroyImage( const sAllocatedImage& _image )
		{
			ZoneScoped;

			const cRenderer_vulkan* renderer = reinterpret_cast< cRenderer_vulkan* >( cRenderer::getRenderInstance() );

			vkDestroyImageView( renderer->logical_device, _image.image_view, nullptr );
			vmaDestroyImage( renderer->memory_allocator, _image.image, _image.allocation );
		}
	}
}
