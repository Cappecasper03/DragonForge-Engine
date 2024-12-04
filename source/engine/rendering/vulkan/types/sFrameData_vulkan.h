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

		const vk::DescriptorSet& getVertexSceneDescriptorSet() const;
		const vk::DescriptorSet& getVertexSceneDescriptorSet3d() const { return m_vertex_scene_descriptor_set_3d; }
		const vk::DescriptorSet& getVertexSceneDescriptorSet2d() const { return m_vertex_scene_descriptor_set_2d; }

		vk::UniqueCommandPool   command_pool;
		vk::UniqueCommandBuffer command_buffer;

		vk::UniqueSemaphore swapchain_semaphore;
		vk::UniqueSemaphore render_semaphore;
		vk::UniqueFence     render_fence;

		sAllocatedBuffer_vulkan vertex_scene_uniform_buffer_3d;
		sAllocatedBuffer_vulkan vertex_scene_uniform_buffer_2d;

		sDescriptorAllocator_vulkan descriptors;

		TracyVkCtx tracy_context;

	protected:
		sDescriptorAllocator_vulkan m_static_descriptors;

		vk::DescriptorSet m_vertex_scene_descriptor_set_3d;
		vk::DescriptorSet m_vertex_scene_descriptor_set_2d;
	};
}
