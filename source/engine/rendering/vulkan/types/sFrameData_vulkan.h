#pragma once

#include <vulkan/vulkan.hpp>

#include <tracy/TracyVulkan.hpp>

#include "engine/rendering/vulkan/descriptor/sDescriptorAllocator_vulkan.h"
#include "sAllocatedBuffer_vulkan.h"

namespace df::vulkan
{
	class cRenderer_vulkan;

	struct sFrameData_vulkan
	{
		void create();
		void create( const cRenderer_vulkan* _renderer );
		void destroy();

		vk::UniqueCommandPool   command_pool;
		vk::UniqueCommandBuffer command_buffer;

		vk::UniqueSemaphore swapchain_semaphore;
		vk::UniqueSemaphore render_semaphore;
		vk::UniqueFence     render_fence;

		sAllocatedBuffer_vulkan vertex_scene_uniform_buffer_3d;
		sAllocatedBuffer_vulkan vertex_scene_uniform_buffer_2d;

		sAllocatedBuffer_vulkan* last_used_scene_uniform_buffer;

		sDescriptorAllocator_vulkan descriptors;

		TracyVkCtx tracy_context;
	};
}
