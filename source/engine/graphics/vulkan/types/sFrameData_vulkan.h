#pragma once

#include <vulkan/vulkan.hpp>

#include "engine/graphics/vulkan/cCommandBuffer.h"
#include "engine/graphics/vulkan/descriptor/sDescriptorAllocator_vulkan.h"
#include "sAllocatedBuffer_vulkan.h"
#include "sSubmitContext_vulkan.h"

namespace df::vulkan
{
	class cRenderer_vulkan;

	struct sFrameData_vulkan
	{
		void create();
		void create( const cRenderer_vulkan* _renderer );
		void destroy();

		const sAllocatedBuffer_vulkan& getVertexSceneBuffer() const;
		const vk::DescriptorSet&       getVertexDescriptorSet() const;

		vk::UniqueCommandPool command_pool;
		cCommandBuffer        command_buffer;

		vk::UniqueSemaphore swapchain_semaphore;
		vk::UniqueSemaphore render_semaphore;
		vk::UniqueFence     render_fence;

		sAllocatedBuffer_vulkan vertex_scene_uniform_buffer_3d;
		sAllocatedBuffer_vulkan vertex_scene_uniform_buffer_2d;

		static vk::UniqueDescriptorSetLayout s_vertex_scene_descriptor_set_layout;
		vk::DescriptorSet                    vertex_scene_descriptor_set_3d;
		vk::DescriptorSet                    vertex_scene_descriptor_set_2d;

		sAllocatedBuffer_vulkan fragment_scene_uniform_buffer;

		static vk::UniqueDescriptorSetLayout s_fragment_scene_descriptor_set_layout;
		vk::DescriptorSet                    fragment_scene_descriptor_set;

		sDescriptorAllocator_vulkan static_descriptors;
		sDescriptorAllocator_vulkan dynamic_descriptors;

		cProfilingContext profiling_context;
	};
}
