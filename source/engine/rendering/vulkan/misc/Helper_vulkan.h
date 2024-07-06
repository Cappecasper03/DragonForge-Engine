#pragma once

#include <string>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

#include "Types_vulkan.h"

namespace df::vulkan::helper
{
	namespace init
	{
		vk::CommandPoolCreateInfo     commandPoolCreateInfo( uint32_t _queue_family_index, vk::CommandPoolCreateFlags _flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer );
		vk::CommandBufferAllocateInfo commandBufferAllocateInfo( vk::CommandPool _command_pool, uint32_t _count = 1 );
		vk::CommandBufferBeginInfo    commandBufferBeginInfo( vk::CommandBufferUsageFlags _flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit );

		vk::FenceCreateInfo     fenceCreateInfo( vk::FenceCreateFlags _flags = vk::FenceCreateFlagBits::eSignaled );
		vk::SemaphoreCreateInfo semaphoreCreateInfo();

		vk::SemaphoreSubmitInfo     semaphoreSubmitInfo( vk::PipelineStageFlags2 _stage_mask, vk::Semaphore _semaphore );
		vk::CommandBufferSubmitInfo commandBufferSubmitInfo( vk::CommandBuffer _command_buffer );

		vk::SubmitInfo2 submitInfo( vk::CommandBufferSubmitInfo* _command_buffer, vk::SemaphoreSubmitInfo* _signal_semaphore_info, vk::SemaphoreSubmitInfo* _wait_semaphore_info );

		vk::PresentInfoKHR presentInfo( vk::Semaphore _semaphore, vk::SwapchainKHR _swapchain, uint32_t _swap_chain_index );

		vk::RenderingAttachmentInfo attachmentInfo( vk::ImageView _view, const vk::ClearValue* _clear, vk::ImageLayout _layout = vk::ImageLayout::eColorAttachmentOptimal );
		vk::RenderingAttachmentInfo depthAttachmentInfo( vk::ImageView _view, vk::ImageLayout _layout = vk::ImageLayout::eColorAttachmentOptimal );

		vk::ImageCreateInfo       imageCreateInfo( vk::Format _format, vk::ImageUsageFlags _usage_flags, vk::Extent3D _extent );
		vk::ImageViewCreateInfo   imageViewCreateInfo( vk::Format _format, vk::Image _image, vk::ImageAspectFlags _aspect_flags );
		vk::ImageSubresourceRange imageSubresourceRange( vk::ImageAspectFlags _aspect_mask );

		vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfo( vk::ShaderStageFlagBits _stage, vk::ShaderModule _module );

		vk::RenderingInfo renderingInfo( vk::Extent2D _extent, const vk::RenderingAttachmentInfo* _color_attachment, const vk::RenderingAttachmentInfo* _depth_attachment );
	}

	namespace util
	{
		void transitionImage( vk::CommandBuffer _command_buffer, vk::Image _image, vk::ImageLayout _current_layout, vk::ImageLayout _new_layout );
		void copyImageToImage( vk::CommandBuffer _command_buffer, vk::Image _source, vk::Image _destination, vk::Extent2D _source_size, vk::Extent2D _destination_size );

		vk::ShaderModule createShaderModule( const std::string& _name );

		void             createBuffer( vk::DeviceSize _size, vk::BufferUsageFlags _usage_flags, VmaMemoryUsage _memory_usage, sAllocatedBuffer& _buffer );
		void             createBuffer( vk::DeviceSize _size, vk::BufferUsageFlags _usage_flags, VmaMemoryUsage _memory_usage, sAllocatedBuffer& _buffer, VmaAllocator _memory_allocator );
		sAllocatedBuffer createBuffer( vk::DeviceSize _size, vk::BufferUsageFlags _usage_flags, VmaMemoryUsage _memory_usage );
		sAllocatedBuffer createBuffer( vk::DeviceSize _size, vk::BufferUsageFlags _usage_flags, VmaMemoryUsage _memory_usage, VmaAllocator _memory_allocator );
		void             destroyBuffer( const sAllocatedBuffer& _buffer );
		void             destroyBuffer( const sAllocatedBuffer& _buffer, VmaAllocator _memory_allocator );

		void            createImage( vk::Extent3D _size, vk::Format _format, vk::ImageUsageFlags _usage, sAllocatedImage& _image, bool _mipmapped = false, unsigned _mipmaps = 0 );
		sAllocatedImage createImage( vk::Extent3D _size, vk::Format _format, vk::ImageUsageFlags _usage, bool _mipmapped = false, unsigned _mipmaps = 0 );
		sAllocatedImage createImage( const void* _data, vk::Extent3D _size, vk::Format _format, vk::ImageUsageFlags _usage, bool _mipmapped = false, unsigned _mipmaps = 0 );
		void            destroyImage( const sAllocatedImage& _image );
	}
}
