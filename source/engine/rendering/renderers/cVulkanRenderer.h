#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "iRenderer.h"
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

    private:
        bool        createInstance();
        static bool isAllValidationLayersFound();

        bool createDebugMessenger();
        bool destroyDebugMessenger() const;

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback( VkDebugUtilsMessageSeverityFlagBitsEXT      _message_severity,
                                                                    VkDebugUtilsMessageTypeFlagsEXT             _message_type,
                                                                    const VkDebugUtilsMessengerCallbackDataEXT* _callback_data,
                                                                    void*                                       _user_data );

        GLFWwindow* m_window;
        VkInstance  m_instance;

        VkDebugUtilsMessengerEXT m_debug_messenger;
    };
}
