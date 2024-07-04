#pragma once

#include <functional>
#include <vector>
#include <vk_mem_alloc.h>
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

		void immediateSubmit( std::function< void( VkCommandBuffer ) >&& _function ) const;

		void setViewport();
		void setScissor();
		void setViewportScissor();

		void initializeImGui() override;

		VkExtent2D getRenderExtent() const { return m_render_extent; }
		VkFormat   getRenderColorFormat() const { return m_render_image.format; }
		VkFormat   getRenderDepthFormat() const { return m_depth_image.format; }

		sFrameData& getCurrentFrame() { return m_frames[ m_frame_number % frame_overlap ]; }

		VkPhysicalDevice physical_device;
		VkDevice         logical_device;
		VmaAllocator     memory_allocator;

		VkDescriptorSetLayout vertex_scene_uniform_layout;

		VkSampler sampler_linear;
		VkSampler sampler_nearest;

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

		VkInstance m_instance;

		VkQueue  m_graphics_queue;
		uint32_t m_graphics_queue_family;

		VkSurfaceKHR m_surface;

		sAllocatedImage m_depth_image;
		sAllocatedImage m_render_image;
		VkExtent2D      m_render_extent;

		VkSwapchainKHR             m_swapchain;
		std::vector< VkImage >     m_swapchain_images;
		std::vector< VkImageView > m_swapchain_image_views;
		VkFormat                   m_swapchain_format;
		VkExtent2D                 m_swapchain_extent;

		static constexpr int      frame_overlap = 3;
		int                       m_frame_number;
		std::vector< sFrameData > m_frames;

		sSubmitContext m_submit_context;

		VkDescriptorPool m_imgui_descriptor_pool;

		VkDebugUtilsMessengerEXT m_debug_messenger;
	};
}
