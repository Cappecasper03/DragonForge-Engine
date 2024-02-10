#include "cVulkanRenderer.h"

#define GLFW_INCLUDE_VULKAN

#include <format>
#include <GLFW/glfw3.h>

#include "framework/application/cApplication.h"

namespace df
{
    std::vector< const char* > cVulkanRenderer::validation_layers = { "VK_LAYER_KHRONOS_validation" };

    cVulkanRenderer::cVulkanRenderer()
    {
        ZoneScoped;

        if( m_glfw_use_count == 0 )
        {
            glfwInit();
            DF_LOG_MESSAGE( "Initialized GLFW" );
        }
        m_glfw_use_count++;

        glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
        glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

        m_window = glfwCreateWindow( m_window_size.x, m_window_size.y, cApplication::getName().c_str(), nullptr, nullptr );
        if( !m_window )
        {
            DF_LOG_ERROR( "Failed to create window" );
            return;
        }
        DF_LOG_MESSAGE( std::format( "Created window [{}, {}]", m_window_size.x, m_window_size.y ) );

        if( !createInstance() )
            return;
    }

    cVulkanRenderer::~cVulkanRenderer()
    {
        ZoneScoped;

        vkDestroyInstance( m_instance, nullptr );
        glfwDestroyWindow( m_window );

        m_glfw_use_count--;
        if( m_glfw_use_count == 0 )
        {
            glfwTerminate();
            DF_LOG_MESSAGE( "Deinitialized GLFW" );
        }
    }

    std::vector< const char* > cVulkanRenderer::getRequiredExtensions()
    {
        uint32_t     extension_count = 0;
        const char** req_extensions  = glfwGetRequiredInstanceExtensions( &extension_count );

        std::vector extensions( req_extensions, req_extensions + extension_count );

#ifdef DEBUG
        extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
#endif

        return extensions;
    }

    bool cVulkanRenderer::createInstance()
    {
        ZoneScoped;

        bool use_validation_layers = false;
#ifdef DEBUG
        use_validation_layers = isAllValidationLayersFound();
#endif

        VkApplicationInfo application_info{};
        application_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        application_info.pApplicationName   = cApplication::getName().c_str();
        application_info.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
        application_info.pEngineName        = "DragonForge-Engine";
        application_info.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 );
        application_info.apiVersion         = VK_API_VERSION_1_3;

        const std::vector< const char* > required_extensions = getRequiredExtensions();

        VkInstanceCreateInfo create_info{};
        create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo        = &application_info;
        create_info.enabledExtensionCount   = static_cast< uint32_t >( required_extensions.size() );
        create_info.ppEnabledExtensionNames = required_extensions.data();
        if( use_validation_layers )
        {
            create_info.enabledLayerCount   = static_cast< uint32_t >( validation_layers.size() );
            create_info.ppEnabledLayerNames = validation_layers.data();
        }
        else
            create_info.enabledLayerCount = 0;

        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, nullptr );
        std::vector< VkExtensionProperties > extensions( extension_count );
        vkEnumerateInstanceExtensionProperties( nullptr, &extension_count, extensions.data() );

        for( const char* required_extension : required_extensions )
        {
            bool                   found = false;
            const std::string_view req_name( required_extension );

            for( const VkExtensionProperties& extension : extensions )
            {
                if( std::string_view( extension.extensionName ) == req_name )
                {
                    found = true;
                    break;
                }
            }

            if( !found )
            {
                DF_LOG_ERROR( "Not all required extensions was found" );
                return false;
            }
        }

        if( vkCreateInstance( &create_info, nullptr, &m_instance ) )
        {
            DF_LOG_ERROR( "Failed to create instance" );
            return false;
        }

        DF_LOG_MESSAGE( "Created instance" );
        return true;
    }

    bool cVulkanRenderer::isAllValidationLayersFound()
    {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties( &layer_count, nullptr );
        std::vector< VkLayerProperties > layers;
        vkEnumerateInstanceLayerProperties( &layer_count, layers.data() );

        for( const char* wanted_layer : validation_layers )
        {
            bool                   found = false;
            const std::string_view wanted_string( wanted_layer );

            for( const VkLayerProperties& layer : layers )
            {
                if( std::string_view( layer.layerName ) == wanted_string )
                {
                    found = true;
                    break;
                }
            }

            if( !found )
            {
                DF_LOG_WARNING( "Validation layers requested, but not all available" );
                return false;
            }
        }

        return true;
    }
}
