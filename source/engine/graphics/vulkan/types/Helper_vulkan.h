#pragma once

#include <string>
#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

#include "sAllocatedBuffer_vulkan.h"
#include "sAllocatedImage_vulkan.h"

namespace df::vulkan::helper
{
	namespace init
	{
		vk::SubmitInfo2 submitInfo( const vk::CommandBufferSubmitInfo* _command_buffer        = nullptr,
		                            const vk::SemaphoreSubmitInfo*     _signal_semaphore_info = nullptr,
		                            const vk::SemaphoreSubmitInfo*     _wait_semaphore_info   = nullptr );
		vk::SubmitInfo2 submitInfo( const std::vector< vk::CommandBufferSubmitInfo >& _command_buffers        = {},
		                            const std::vector< vk::SemaphoreSubmitInfo >&     _signal_semaphore_infos = {},
		                            const std::vector< vk::SemaphoreSubmitInfo >&     _wait_semaphore_infos   = {} );

		vk::RenderingAttachmentInfo attachmentInfo( const vk::ImageView& _view, const vk::ClearValue* _clear, vk::ImageLayout _layout = vk::ImageLayout::eColorAttachmentOptimal );
		vk::RenderingAttachmentInfo depthAttachmentInfo( const vk::ImageView& _view, vk::ImageLayout _layout = vk::ImageLayout::eColorAttachmentOptimal );
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
	}
}
