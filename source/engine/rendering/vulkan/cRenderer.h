#pragma once

#include <functional>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "assets/sRenderAsset.h"
#include "engine/misc/Misc.h"
#include "engine/rendering/iRenderer.h"

struct GLFWwindow;

namespace df::vulkan
{
	class cRenderer final : public iRenderer
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cRenderer )

		cRenderer();
		~cRenderer() override;

		void render() override;

		void clearBuffers( int _buffers, const cColor& _color ) override;

		void*             getWindow() override { return m_window; }
		const glm::ivec2& getWindowSize() override { return m_window_size; }

		void immediateSubmit( std::function< void( VkCommandBuffer ) >&& _function ) const;

		VkPhysicalDevice physical_device;
		VkDevice         logical_device;
		VmaAllocator     memory_allocator;

	private:
		struct sFrameData
		{
			VkCommandPool   command_pool;
			VkCommandBuffer command_buffer;

			VkSemaphore swapchain_semaphore;
			VkSemaphore render_semaphore;
			VkFence     render_fence;
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

		GLFWwindow* m_window;
		VkInstance  m_instance;

		VkQueue  m_graphics_queue;
		uint32_t m_graphics_queue_family;
		VkQueue  m_present_queue;
		uint32_t m_present_queue_family;

		VkSurfaceKHR m_surface;

		sAllocatedImage m_draw_image;
		VkExtent2D      m_draw_extent;

		VkSwapchainKHR             m_swapchain;
		std::vector< VkImage >     m_swapchain_images;
		std::vector< VkImageView > m_swapchain_image_views;
		VkFormat                   m_swapchain_format;
		VkExtent2D                 m_swapchain_extent;

		static constexpr int      frame_overlap = 2;
		int                       m_frame_number;
		std::vector< sFrameData > m_frames;

		sSubmitContext m_submit_context;

		VkDebugUtilsMessengerEXT m_debug_messenger;
	};
}
