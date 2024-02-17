#pragma once

#include <optional>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "../iRenderer.h"
#include "engine/misc/Misc.h"

struct GLFWwindow;

namespace df
{
    class cVulkanRenderer final : public iRenderer
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cVulkanRenderer )

        cVulkanRenderer();
        ~cVulkanRenderer() override;

        void render() override {}

        void*             getWindow() override { return m_window; }
        const glm::ivec2& getWindowSize() override { return m_window_size; }

        static std::vector< const char* > getRequiredExtensions();

        static std::vector< const char* > validation_layers;
        static std::vector< const char* > device_extenstions;

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

        static bool checkValidationLayers();
        static bool checkDeviceExtensions( const VkPhysicalDevice& _device );

        bool                pickPhysicalDevice();
        int                 rateDeviceSuitability( const VkPhysicalDevice& _device ) const;
        sQueueFamilyIndices findQueueFamilies( const VkPhysicalDevice& _device ) const;

        sSwapChainSupportDetails  querySwapChainSupport( const VkPhysicalDevice& _device ) const;
        static VkSurfaceFormatKHR chooseSwapChainSurfaceFormat( const std::vector< VkSurfaceFormatKHR >& _formats );
        static VkPresentModeKHR   chooseSwapChainPresentMode( const std::vector< VkPresentModeKHR >& _present_modes );
        VkExtent2D                chooseSwapChainExtent( const VkSurfaceCapabilitiesKHR& _capabilities ) const;

        VkResult createDebugMessenger();
        VkResult destroyDebugMessenger() const;

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

        VkSwapchainKHR             m_swap_chain;
        std::vector< VkImage >     m_swap_chain_images;
        std::vector< VkImageView > m_swap_chain_image_views;
        VkFormat                   m_swap_chain_format;
        VkExtent2D                 m_swap_chain_extent;

        VkDebugUtilsMessengerEXT m_debug_messenger;
    };
}
