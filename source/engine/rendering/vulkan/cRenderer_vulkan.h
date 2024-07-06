#pragma once

#include <functional>
#include <vector>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

#include "engine/misc/Misc.h"
#include "engine/rendering/iRenderer.h"
#include "misc/Types_vulkan.h"

namespace df::vulkan
{
	class cRenderer_vulkan final : public iRenderer
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cRenderer_vulkan )

		cRenderer_vulkan();
		~cRenderer_vulkan() override;

		void render() override;

		void beginRendering( int _clear_buffers, const cColor& _color ) override;
		void endRendering() override;

		void immediateSubmit( std::function< void( vk::CommandBuffer ) >&& _function ) const;

		void setViewport();
		void setScissor();
		void setViewportScissor();

		void initializeImGui() override;

		vk::Extent2D getRenderExtent() const { return m_render_extent; }
		vk::Format   getRenderColorFormat() const { return m_render_image.format; }
		vk::Format   getRenderDepthFormat() const { return m_depth_image.format; }

		sFrameData& getCurrentFrame() { return m_frames[ m_frame_number % frame_overlap ]; }

		const vk::PhysicalDevice& getPhysicalDevice() const { return m_physical_device; }
		const vk::UniqueDevice&   getLogicalDevice() const { return m_logical_device; }
		VmaAllocator              memory_allocator;

		const vk::UniqueDescriptorSetLayout& getVertexSceneUniformLayout() const { return m_vertex_scene_uniform_layout; }

		const vk::UniqueSampler& getLinearSampler() const { return m_sampler_linear; }
		const vk::UniqueSampler& getNearestSampler() const { return m_sampler_nearest; }

	private:
		void createSwapchain( uint32_t _width, uint32_t _height );
		void createFrameDatas();
		void createMemoryAllocator();
		void createSubmitContext();

		void resize();

		static void framebufferSizeCallback( GLFWwindow* _window, int _width, int _height );

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback( VkDebugUtilsMessageSeverityFlagBitsEXT      _message_severity,
		                                                            VkDebugUtilsMessageTypeFlagsEXT             _message_type,
		                                                            const VkDebugUtilsMessengerCallbackDataEXT* _callback_data,
		                                                            void*                                       _user_data );

		vk::UniqueInstance m_instance;

		vk::Queue m_graphics_queue;
		uint32_t  m_graphics_queue_family;

		vk::SurfaceKHR m_surface;

		sAllocatedImage m_depth_image;
		sAllocatedImage m_render_image;
		vk::Extent2D    m_render_extent;

		vk::UniqueSwapchainKHR             m_swapchain;
		std::vector< vk::UniqueImage >     m_swapchain_images;
		std::vector< vk::UniqueImageView > m_swapchain_image_views;
		vk::Format                         m_swapchain_format;
		vk::Extent2D                       m_swapchain_extent;

		vk::PhysicalDevice m_physical_device;
		vk::UniqueDevice   m_logical_device;

		vk::UniqueDescriptorSetLayout m_vertex_scene_uniform_layout;

		vk::UniqueSampler m_sampler_linear;
		vk::UniqueSampler m_sampler_nearest;

		static constexpr int      frame_overlap = 3;
		int                       m_frame_number;
		std::vector< sFrameData > m_frames;

		sSubmitContext m_submit_context;

		vk::UniqueDescriptorPool m_imgui_descriptor_pool;

		vk::UniqueDebugUtilsMessengerEXT m_debug_messenger;
	};
}
