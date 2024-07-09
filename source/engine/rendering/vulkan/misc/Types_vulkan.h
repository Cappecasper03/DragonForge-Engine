#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

#include "engine/rendering/vulkan/descriptor/sDescriptorAllocator_vulkan.h"

namespace df::vulkan
{
	struct sAllocatedBuffer_vulkan
	{
		vma::UniqueBuffer     buffer;
		vma::UniqueAllocation allocation;
		vma::AllocationInfo   allocation_info;
	};

	struct sFrameData_vulkan
	{
		vk::UniqueCommandPool   command_pool;
		vk::UniqueCommandBuffer command_buffer;

		vk::UniqueSemaphore swapchain_semaphore;
		vk::UniqueSemaphore render_semaphore;
		vk::UniqueFence     render_fence;

		sAllocatedBuffer_vulkan     vertex_scene_uniform_buffer;
		sDescriptorAllocator_vulkan descriptors;
	};

	struct sAllocatedImage_vulkan
	{
		vma::UniqueImage      image;
		vk::UniqueImageView   image_view;
		vma::UniqueAllocation allocation;
		vk::Extent3D          extent;
		vk::Format            format;
	};

	struct sSubmitContext_vulkan
	{
		vk::UniqueFence         fence;
		vk::UniqueCommandPool   command_pool;
		vk::UniqueCommandBuffer command_buffer;
	};

	struct sVertexSceneUniforms_vulkan
	{
		glm::mat4 view_projection;
	};
};
