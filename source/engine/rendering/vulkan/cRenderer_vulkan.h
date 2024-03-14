#pragma once

#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "assets/sRenderAsset_vulkan.h"
#include "engine/misc/Misc.h"
#include "engine/rendering/iRenderer.h"
#include "misc/sDescriptorAllocator_vulkan.h"

namespace df::vulkan
{
	class cRenderer_vulkan final : public iRenderer
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cRenderer_vulkan )

		struct sSceneConstants
		{
			glm::mat4 view;
			glm::mat4 projection;
			glm::mat4 view_projection;
		};

		cRenderer_vulkan();
		~cRenderer_vulkan() override;

		void render() override;

		void beginRendering( int _clear_buffers, const cColor& _color ) override;
		void endRendering() override;

		void immediateSubmit( std::function< void( VkCommandBuffer ) >&& _function ) const;

		VkExtent2D getRenderExtent() const { return m_render_extent; }
		VkFormat   getRenderColorFormat() const { return m_render_image.format; }
		VkFormat   getRenderDepthFormat() const { return m_depth_image.format; }

		const sSceneConstants& getSceneConstants() const { return m_scene_constants; }

		VkPhysicalDevice physical_device;
		VkDevice         logical_device;
		VmaAllocator     memory_allocator;

		VkCommandBuffer current_render_command_buffer;

	private:
		struct sFrameData
		{
			VkCommandPool   command_pool;
			VkCommandBuffer command_buffer;

			VkSemaphore swapchain_semaphore;
			VkSemaphore render_semaphore;
			VkFence     render_fence;

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

		sFrameData& getCurrentFrame() { return m_frames[ m_frame_number % frame_overlap ]; }

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
		VkQueue  m_present_queue;
		uint32_t m_present_queue_family;

		VkSurfaceKHR m_surface;

		sAllocatedImage m_depth_image;
		sAllocatedImage m_render_image;
		VkExtent2D      m_render_extent;

		VkSwapchainKHR             m_swapchain;
		std::vector< VkImage >     m_swapchain_images;
		std::vector< VkImageView > m_swapchain_image_views;
		VkFormat                   m_swapchain_format;
		VkExtent2D                 m_swapchain_extent;

		static constexpr int      frame_overlap = 2;
		int                       m_frame_number;
		std::vector< sFrameData > m_frames;

		sSceneConstants       m_scene_constants;
		VkDescriptorSetLayout m_scene_constants_descriptor_layout;

		sSubmitContext m_submit_context;

		VkDebugUtilsMessengerEXT m_debug_messenger;
	};
}
