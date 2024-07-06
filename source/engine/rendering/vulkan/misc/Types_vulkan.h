#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

#include "engine/rendering/vulkan/descriptor/sDescriptorAllocator_vulkan.h"

namespace df::vulkan
{
	struct sAllocatedBuffer
	{
		vk::UniqueBuffer  buffer;
		VmaAllocation     allocation;
		VmaAllocationInfo allocation_info;
	};

	struct sFrameData
	{
		vk::UniqueCommandPool   command_pool;
		vk::UniqueCommandBuffer command_buffer;

		vk::UniqueSemaphore swapchain_semaphore;
		vk::UniqueSemaphore render_semaphore;
		vk::UniqueFence     render_fence;

		sAllocatedBuffer            vertex_scene_uniform_buffer;
		sDescriptorAllocator_vulkan descriptors;
	};

	struct sAllocatedImage
	{
		vk::UniqueImage     image;
		vk::UniqueImageView image_view;
		VmaAllocation       allocation;
		vk::Extent3D        extent;
		vk::Format          format;
	};

	struct sSubmitContext
	{
		vk::UniqueFence         fence;
		vk::UniqueCommandPool   command_pool;
		vk::UniqueCommandBuffer command_buffer;
	};

	struct sVertexSceneUniforms
	{
		glm::mat4 view_projection;
	};
};
