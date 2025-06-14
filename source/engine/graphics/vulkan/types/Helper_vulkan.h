﻿#pragma once

#include <string>
#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

#include "sAllocatedBuffer_vulkan.h"
#include "sAllocatedImage_vulkan.h"

namespace df::vulkan::helper
{
	namespace init
	{
		vk::CommandPoolCreateInfo commandPoolCreateInfo( uint32_t _queue_family_index, vk::CommandPoolCreateFlags _flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer );

		vk::FenceCreateInfo fenceCreateInfo( vk::FenceCreateFlags _flags = vk::FenceCreateFlagBits::eSignaled );

		vk::SemaphoreCreateInfo semaphoreCreateInfo();

		vk::SemaphoreSubmitInfo semaphoreSubmitInfo( vk::PipelineStageFlags2 _stage_mask, const vk::Semaphore& _semaphore );

		vk::CommandBufferSubmitInfo commandBufferSubmitInfo( const vk::CommandBuffer& _command_buffer );

		vk::SubmitInfo2 submitInfo( const vk::CommandBufferSubmitInfo* _command_buffer        = nullptr,
		                            const vk::SemaphoreSubmitInfo*     _signal_semaphore_info = nullptr,
		                            const vk::SemaphoreSubmitInfo*     _wait_semaphore_info   = nullptr );
		vk::SubmitInfo2 submitInfo( const std::vector< vk::CommandBufferSubmitInfo >& _command_buffers        = {},
		                            const std::vector< vk::SemaphoreSubmitInfo >&     _signal_semaphore_infos = {},
		                            const std::vector< vk::SemaphoreSubmitInfo >&     _wait_semaphore_infos   = {} );

		vk::PresentInfoKHR presentInfo( const vk::Semaphore* _semaphore = nullptr, const vk::SwapchainKHR* _swapchain = nullptr, const uint32_t* _swap_chain_index = nullptr );
		vk::PresentInfoKHR presentInfo( const std::vector< vk::Semaphore >&    _semaphores         = {},
		                                const std::vector< vk::SwapchainKHR >& _swapchains         = {},
		                                const std::vector< uint32_t >&         _swap_chain_indices = {} );

		vk::RenderingAttachmentInfo attachmentInfo( const vk::ImageView& _view, const vk::ClearValue* _clear, vk::ImageLayout _layout = vk::ImageLayout::eColorAttachmentOptimal );
		vk::RenderingAttachmentInfo depthAttachmentInfo( const vk::ImageView& _view, vk::ImageLayout _layout = vk::ImageLayout::eColorAttachmentOptimal );

		vk::ImageSubresourceRange imageSubresourceRange( vk::ImageAspectFlags _aspect_mask );

		vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfo( vk::ShaderStageFlagBits _stage, const vk::ShaderModule& _module );
	}

	namespace util
	{
		void transitionImage( const vk::CommandBuffer& _command_buffer, const vk::Image& _image, vk::ImageLayout _current_layout, vk::ImageLayout _new_layout );

		void copyImageToImage( const vk::CommandBuffer& _command_buffer,
		                       const vk::Image&         _source,
		                       const vk::Image&         _destination,
		                       vk::Extent2D             _source_size,
		                       vk::Extent2D             _destination_size );

		vk::ShaderModule createShaderModule( const std::string& _name );

		void                    createBuffer( vk::DeviceSize _size, vk::BufferUsageFlags _usage_flags, vma::MemoryUsage _memory_usage, sAllocatedBuffer_vulkan& _buffer );
		void                    createBuffer( vk::DeviceSize           _size,
		                                      vk::BufferUsageFlags     _usage_flags,
		                                      vma::MemoryUsage         _memory_usage,
		                                      sAllocatedBuffer_vulkan& _buffer,
		                                      const vma::Allocator&    _memory_allocator );
		sAllocatedBuffer_vulkan createBuffer( vk::DeviceSize _size, vk::BufferUsageFlags _usage_flags, vma::MemoryUsage _memory_usage );
		sAllocatedBuffer_vulkan createBuffer( vk::DeviceSize _size, vk::BufferUsageFlags _usage_flags, vma::MemoryUsage _memory_usage, const vma::Allocator& _memory_allocator );

		void setBufferData( void const* _data, size_t _data_size, size_t _offset, const sAllocatedBuffer_vulkan& _buffer, bool _copy = false );
		void setBufferData( void const*                    _data,
		                    size_t                         _data_size,
		                    size_t                         _offset,
		                    const sAllocatedBuffer_vulkan& _buffer,
		                    const vma::Allocator&          _memory_allocator,
		                    bool                           _copy = false );

		void destroyBuffer( sAllocatedBuffer_vulkan& _buffer );
		void destroyBuffer( sAllocatedBuffer_vulkan& _buffer, vma::Allocator& _memory_allocator );

		void destroyImage( sAllocatedImage_vulkan& _image );
	}
}
