#pragma once

#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../iRenderer.h"
#include "engine/misc/Misc.h"

struct GLFWwindow;

namespace df::vulkan
{
    struct sRendererSpecific;

    class cRenderer final : public iRenderer
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cRenderer )

        friend cQuad;
        friend sRendererSpecific;

        cRenderer();
        ~cRenderer() override;

        void render() override;

        void*             getWindow() override { return m_window; }
        const glm::ivec2& getWindowSize() override { return m_window_size; }

        static std::vector< const char* > getRequiredExtensions();

        std::vector< const char* > validation_layers;
        std::vector< const char* > device_extenstions;

        std::vector< VkVertexInputBindingDescription >   vertex_input_binding_descriptions;
        std::vector< VkVertexInputAttributeDescription > vertex_input_attribute_descriptions;

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

        bool createInstance();
        bool createLogicalDevice();
        bool createSwapChain();
        bool createImageViews();
        bool createRenderPass();
        bool createGraphicsPipeline();
        bool createFramebuffers();
        bool createCommandPool();
        bool createCommandBuffers();
        bool createSyncObjects();

        bool recreateSwapChain();
        bool recreateGraphicsPipeline();

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

        static std::vector< char > loadShader( const std::string& _name );
        VkShaderModule             createShaderModule( const std::vector< char >& _shader ) const;

        VkResult createDebugMessenger();
        VkResult destroyDebugMessenger() const;

        static void framebufferSizeCallback( GLFWwindow* _window, int _width, int _height );

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback( VkDebugUtilsMessageSeverityFlagBitsEXT      _message_severity,
                                                                    VkDebugUtilsMessageTypeFlagsEXT             _message_type,
                                                                    const VkDebugUtilsMessengerCallbackDataEXT* _callback_data,
                                                                    void*                                       _user_data );

        GLFWwindow* m_window;
        VkInstance  m_instance;

        VkPhysicalDevice m_physical_device;
        VkDevice         m_logical_device;

        VkQueue m_graphics_queue;
        VkQueue m_present_queue;

        VkSurfaceKHR m_surface;

        VkSwapchainKHR               m_swap_chain;
        std::vector< VkImage >       m_swap_chain_images;
        std::vector< VkImageView >   m_swap_chain_image_views;
        VkFormat                     m_swap_chain_format;
        VkExtent2D                   m_swap_chain_extent;
        std::vector< VkFramebuffer > m_swap_chain_framebuffers;

        VkRenderPass     m_render_pass;
        VkPipelineLayout m_pipeline_layout;
        VkPipeline       m_pipeline;

        VkCommandPool                  m_command_pool;
        std::vector< VkCommandBuffer > m_command_buffers;

        std::vector< VkSemaphore > m_image_available_semaphores;
        std::vector< VkSemaphore > m_render_finish_semaphores;
        std::vector< VkFence >     m_rendering_fences;

        VkDebugUtilsMessengerEXT m_debug_messenger;

        int                  m_current_frame;
        static constexpr int max_frames_rendering = 2;
    };
}
