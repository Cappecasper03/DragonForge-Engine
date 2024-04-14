#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <vk_mem_alloc.h>

#include "engine/rendering/vulkan/descriptor/sDescriptorAllocator_vulkan.h"

namespace df::vulkan
{
	struct sAllocatedBuffer
	{
		VkBuffer          buffer;
		VmaAllocation     allocation;
		VmaAllocationInfo allocation_info;
	};

	struct sFrameData
	{
		VkCommandPool   command_pool;
		VkCommandBuffer command_buffer;

		VkSemaphore swapchain_semaphore;
		VkSemaphore render_semaphore;
		VkFence     render_fence;

		sAllocatedBuffer            vertex_scene_buffer;
		sDescriptorAllocator_vulkan descriptors;
	};

	struct sAllocatedImage
	{
		VkImage       image;
		VkImageView   image_view;
		VmaAllocation allocation;
		VkExtent3D    extent;
		VkFormat      format;
	};

	struct sSubmitContext
	{
		VkFence         fence;
		VkCommandPool   command_pool;
		VkCommandBuffer command_buffer;
	};

	struct sVertexSceneUniforms
	{
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 view_projection;
	};
};
