#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "assets/sRenderAsset.h"
#include "engine/misc/Misc.h"
#include "engine/rendering/iRenderer.h"

namespace df::vulkan
{
	class cPipeline;
}

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

		void*             getWindow() override { return m_window; }
		const glm::ivec2& getWindowSize() override { return m_window_size; }

		static std::vector< const char* > getRequiredExtensions();

		VkShaderModule createShaderModule( const std::string& _name ) const;
		bool           createBuffer( VkDeviceSize _size, VkBufferUsageFlags _usage_flags, VmaMemoryUsage _memory_usage, sRenderAsset::sBuffer& _buffer ) const;

		void submit( std::function< void( VkCommandBuffer ) >&& _function ) const;

		std::vector< const char* > validation_layers;
		std::vector< const char* > device_extenstions;

		VkPhysicalDevice physical_device;
		VkDevice         logical_device;
		VkRenderPass     render_pass;

		VmaAllocator memory_allocator;

	private:
		struct sQueueFamilyIndices
		{
			std::optional< uint32_t > graphics;
			std::optional< uint32_t > present;

			bool isComplete() const { return graphics.has_value() && present.has_value(); }
		};

		struct sSwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR          capabilities;
			std::vector< VkSurfaceFormatKHR > formats;
			std::vector< VkPresentModeKHR >   present_modes;

			bool isSupported() const { return !formats.empty() && !present_modes.empty(); }
		};

		struct sSubmitContext
		{
			VkFence         fence;
			VkCommandPool   command_pool;
			VkCommandBuffer command_buffer;
		};

		cPipeline* m_pipeline;

		bool createInstance();
		bool createLogicalDevice();
		bool createSwapChain();
		bool createImageViews();
		bool createRenderPass();
		bool createFramebuffers();
		bool createCommandPool();
		bool createCommandBuffers();
		bool createSyncObjects();
		bool createMemoryAllocator();
		bool createSubmitContext();

		bool recreateSwapChain();

		void recordCommandBuffer( VkCommandBuffer _buffer, uint32_t _image_index ) const;

		bool checkValidationLayers();
		bool checkDeviceExtensions( const VkPhysicalDevice& _device );

		bool                pickPhysicalDevice();
		int                 rateDeviceSuitability( const VkPhysicalDevice& _device );
		sQueueFamilyIndices findQueueFamilies( const VkPhysicalDevice& _device ) const;

		sSwapChainSupportDetails  querySwapChainSupport( const VkPhysicalDevice& _device ) const;
		static VkSurfaceFormatKHR chooseSwapChainSurfaceFormat( const std::vector< VkSurfaceFormatKHR >& _formats );
		static VkPresentModeKHR   chooseSwapChainPresentMode( const std::vector< VkPresentModeKHR >& _present_modes );
		VkExtent2D                chooseSwapChainExtent( const VkSurfaceCapabilitiesKHR& _capabilities ) const;

		VkResult createDebugMessenger();
		VkResult destroyDebugMessenger() const;

		static void framebufferSizeCallback( GLFWwindow* _window, int _width, int _height );

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback( VkDebugUtilsMessageSeverityFlagBitsEXT      _message_severity,
		                                                            VkDebugUtilsMessageTypeFlagsEXT             _message_type,
		                                                            const VkDebugUtilsMessengerCallbackDataEXT* _callback_data,
		                                                            void*                                       _user_data );

		GLFWwindow* m_window;
		VkInstance  m_instance;

		VkQueue m_graphics_queue;
		VkQueue m_present_queue;

		VkSurfaceKHR m_surface;

		VkSwapchainKHR               m_swap_chain;
		std::vector< VkImage >       m_swap_chain_images;
		std::vector< VkImageView >   m_swap_chain_image_views;
		VkFormat                     m_swap_chain_format;
		VkExtent2D                   m_swap_chain_extent;
		std::vector< VkFramebuffer > m_swap_chain_framebuffers;

		VkCommandPool                  m_command_pool;
		std::vector< VkCommandBuffer > m_command_buffers;

		std::vector< VkSemaphore > m_image_available_semaphores;
		std::vector< VkSemaphore > m_render_finish_semaphores;
		std::vector< VkFence >     m_rendering_fences;

		sSubmitContext m_submit_context;

		VkDebugUtilsMessengerEXT m_debug_messenger;

		int                  m_current_frame;
		static constexpr int frame_overlap = 2;
	};
}
