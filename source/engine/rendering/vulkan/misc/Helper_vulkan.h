#pragma once

#include <string>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

#include "Types_vulkan.h"

namespace df::vulkan::helper
{
	namespace init
	{
		VkCommandPoolCreateInfo     commandPoolCreateInfo( uint32_t _queue_family_index, VkCommandPoolCreateFlags _flags = 0 );
		VkCommandBufferAllocateInfo commandBufferAllocateInfo( VkCommandPool _command_pool, uint32_t _count = 1 );
		VkCommandBufferBeginInfo    commandBufferBeginInfo( VkCommandBufferUsageFlags _flags = 0 );

		VkFenceCreateInfo     fenceCreateInfo( VkFenceCreateFlags _flags = 0 );
		VkSemaphoreCreateInfo semaphoreCreateInfo( VkSemaphoreCreateFlags _flags = 0 );

		VkSemaphoreSubmitInfo     semaphoreSubmitInfo( VkPipelineStageFlags2 _stage_mask, VkSemaphore _semaphore );
		VkCommandBufferSubmitInfo commandBufferSubmitInfo( VkCommandBuffer _command_buffer );
		VkSubmitInfo2             submitInfo( VkCommandBufferSubmitInfo* _command_buffer, VkSemaphoreSubmitInfo* _signal_semaphore_info, VkSemaphoreSubmitInfo* _wait_semaphore_info );

		VkPresentInfoKHR presentInfo();

		VkRenderingAttachmentInfo attachmentInfo( VkImageView _view, const VkClearValue* _clear, VkImageLayout _layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );
		VkRenderingAttachmentInfo depthAttachmentInfo( VkImageView _view, VkImageLayout _layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL );

		VkImageCreateInfo       imageCreateInfo( VkFormat _format, VkImageUsageFlags _usage_flags, VkExtent3D _extent );
		VkImageViewCreateInfo   imageViewCreateInfo( VkFormat _format, VkImage _image, VkImageAspectFlags _aspect_flags );
		VkImageSubresourceRange imageSubresourceRange( VkImageAspectFlags _aspect_mask );

		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo( VkShaderStageFlagBits _stage, VkShaderModule _module );

		VkRenderingInfo renderingInfo( VkExtent2D _extent, const VkRenderingAttachmentInfo& _color_attachment, const VkRenderingAttachmentInfo& _depth_attachment );
	}

	namespace util
	{
		void transitionImage( VkCommandBuffer _command_buffer, VkImage _image, VkImageLayout _current_layout, VkImageLayout _new_layout );
		void copyImageToImage( VkCommandBuffer _command_buffer, VkImage _source, VkImage _destination, VkExtent2D _source_size, VkExtent2D _destination_size );

		VkShaderModule createShaderModule( const std::string& _name );

		void             createBuffer( VkDeviceSize _size, VkBufferUsageFlags _usage_flags, VmaMemoryUsage _memory_usage, sAllocatedBuffer& _buffer );
		void             createBuffer( VkDeviceSize _size, VkBufferUsageFlags _usage_flags, VmaMemoryUsage _memory_usage, sAllocatedBuffer& _buffer, VmaAllocator _memory_allocator );
		sAllocatedBuffer createBuffer( VkDeviceSize _size, VkBufferUsageFlags _usage_flags, VmaMemoryUsage _memory_usage );
		sAllocatedBuffer createBuffer( VkDeviceSize _size, VkBufferUsageFlags _usage_flags, VmaMemoryUsage _memory_usage, VmaAllocator _memory_allocator );
		void             destroyBuffer( const sAllocatedBuffer& _buffer );
		void             destroyBuffer( const sAllocatedBuffer& _buffer, VmaAllocator _memory_allocator );

		void            createImage( VkExtent3D _size, VkFormat _format, VkImageUsageFlags _usage, sAllocatedImage& _image, bool _mipmapped = false, unsigned _mipmaps = 0 );
		sAllocatedImage createImage( VkExtent3D _size, VkFormat _format, VkImageUsageFlags _usage, bool _mipmapped = false, unsigned _mipmaps = 0 );
		sAllocatedImage createImage( const void* _data, VkExtent3D _size, VkFormat _format, VkImageUsageFlags _usage, bool _mipmapped = false, unsigned _mipmaps = 0 );
		void            destroyImage( const sAllocatedImage& _image );
	}
}
