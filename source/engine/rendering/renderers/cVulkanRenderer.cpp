#include "cVulkanRenderer.h"

#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32

#include <format>
#include <map>
#include <set>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "framework/application/cApplication.h"

namespace df
{
    std::vector< const char* > cVulkanRenderer::validation_layers  = { "VK_LAYER_KHRONOS_validation" };
    std::vector< const char* > cVulkanRenderer::device_extenstions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    cVulkanRenderer::cVulkanRenderer()
    : m_window( nullptr ),
      m_instance(),
      m_physical_device(),
      m_logical_device(),
      m_graphics_queue(),
      m_present_queue(),
      m_surface(),
      m_debug_messenger()
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

        createDebugMessenger();

        if( !pickPhysicalDevice() || !createLogicalDevice() )
            return;

        if( glfwCreateWindowSurface( m_instance, m_window, nullptr, &m_surface ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to create surface" );
            return;
        }
    }

    cVulkanRenderer::~cVulkanRenderer()
    {
        ZoneScoped;

        if( m_surface )
            vkDestroySurfaceKHR( m_instance, m_surface, nullptr );

        if( m_logical_device )
            vkDestroyDevice( m_logical_device, nullptr );

        if( m_debug_messenger )
            destroyDebugMessenger();

        if( m_instance )
            vkDestroyInstance( m_instance, nullptr );

        if( m_window )
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
        ZoneScoped;

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
        use_validation_layers = checkValidationLayers();
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

#ifdef DEBUG
        VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
        debug_create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        debug_create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
        debug_create_info.pfnUserCallback = debugMessageCallback;
        debug_create_info.pUserData       = this;

        create_info.pNext = &debug_create_info;
#endif

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

        if( vkCreateInstance( &create_info, nullptr, &m_instance ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to create instance" );
            return false;
        }

        DF_LOG_MESSAGE( "Created instance" );
        return true;
    }

    bool cVulkanRenderer::createLogicalDevice()
    {
        ZoneScoped;

        const sQueueFamilyIndices indices        = findQueueFamilies( m_physical_device );
        constexpr float           queue_priority = 1;

        std::vector< VkDeviceQueueCreateInfo > queue_create_infos;

        for( uint32_t family : { indices.graphics.value(), indices.present.value() } )
        {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = family;
            queue_create_info.queueCount       = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back( queue_create_info );
        }

        constexpr VkPhysicalDeviceFeatures device_features{};

        VkDeviceCreateInfo create_info;
        create_info.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos    = queue_create_infos.data();
        create_info.queueCreateInfoCount = static_cast< uint32_t >( queue_create_infos.size() );
        create_info.pEnabledFeatures     = &device_features;

        bool use_validation_layers = false;
#ifdef DEBUG
        use_validation_layers = checkValidationLayers();
#endif

        if( use_validation_layers )
        {
            create_info.enabledLayerCount   = static_cast< uint32_t >( validation_layers.size() );
            create_info.ppEnabledLayerNames = validation_layers.data();
        }
        else
            create_info.enabledLayerCount = 0;

        if( vkCreateDevice( m_physical_device, &create_info, nullptr, &m_logical_device ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to create logical device" );
            return false;
        }

        vkGetDeviceQueue( m_logical_device, indices.graphics.value(), 0, &m_graphics_queue );
        vkGetDeviceQueue( m_logical_device, indices.present.value(), 0, &m_present_queue );

        return true;
    }

    bool cVulkanRenderer::checkValidationLayers()
    {
        ZoneScoped;

        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties( &layer_count, nullptr );
        std::vector< VkLayerProperties > layers;
        vkEnumerateInstanceLayerProperties( &layer_count, layers.data() );

        std::set< std::string > req_layers( validation_layers.begin(), validation_layers.end() );

        for( const VkLayerProperties& extension : layers )
            req_layers.erase( extension.layerName );

        if( !req_layers.empty() )
        {
            DF_LOG_WARNING( "Validation layers requested, but not all available" );
            return false;
        }

        return true;
    }

    bool cVulkanRenderer::checkDeviceExtensions( const VkPhysicalDevice& _device )
    {
        ZoneScoped;

        uint32_t extension_count = 0;
        vkEnumerateDeviceExtensionProperties( _device, nullptr, &extension_count, nullptr );
        std::vector< VkExtensionProperties > extensions( extension_count );
        vkEnumerateDeviceExtensionProperties( _device, nullptr, &extension_count, extensions.data() );

        std::set< std::string > req_extensions( device_extenstions.begin(), device_extenstions.end() );

        for( const VkExtensionProperties& extension : extensions )
            req_extensions.erase( extension.extensionName );

        return req_extensions.empty();
    }

    bool cVulkanRenderer::pickPhysicalDevice()
    {
        ZoneScoped;

        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices( m_instance, &device_count, nullptr );

        if( device_count == 0 )
        {
            DF_LOG_ERROR( "No GPUs with support for Vulkan" );
            return false;
        }

        std::vector< VkPhysicalDevice > devices;
        vkEnumeratePhysicalDevices( m_instance, &device_count, devices.data() );

        std::map< int, VkPhysicalDevice > rated_devices;
        for( const VkPhysicalDevice& device : devices )
            rated_devices.insert( std::make_pair( rateDeviceSuitability( device ), device ) );

        if( rated_devices.empty() || rated_devices.rbegin()->first <= 0 )
        {
            DF_LOG_ERROR( "Failed to find suitable GPU" );
            return false;
        }

        m_physical_device = rated_devices.rbegin()->second;
        return true;
    }

    int cVulkanRenderer::rateDeviceSuitability( const VkPhysicalDevice& _device ) const
    {
        ZoneScoped;

        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties( _device, &device_properties );

        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceFeatures( _device, &device_features );

        if( !device_features.geometryShader || !findQueueFamilies( _device ).isComplete() || !checkDeviceExtensions( _device ) )
            return 0;

        int score = 0;

        if( device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
            score += 1000;

        score += device_properties.limits.maxImageDimension2D;

        return score;
    }

    cVulkanRenderer::sQueueFamilyIndices cVulkanRenderer::findQueueFamilies( const VkPhysicalDevice& _device ) const
    {
        ZoneScoped;

        sQueueFamilyIndices indices;

        uint32_t queue_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties( _device, &queue_count, nullptr );

        std::vector< VkQueueFamilyProperties > queue_families( queue_count );
        vkGetPhysicalDeviceQueueFamilyProperties( _device, &queue_count, queue_families.data() );

        for( uint32_t i = 0; i < static_cast< uint32_t >( queue_families.size() ); ++i )
        {
            if( queue_families[ i ].queueFlags & VK_QUEUE_GRAPHICS_BIT )
                indices.graphics = i;

            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR( _device, i, m_surface, &present_support );

            if( present_support )
                indices.present = i;

            if( indices.isComplete() )
                break;;
        }

        return indices;
    }

    VkResult cVulkanRenderer::createDebugMessenger()
    {
#ifdef DEBUG
        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
        create_info.pfnUserCallback = debugMessageCallback;
        create_info.pUserData       = this;

        const PFN_vkCreateDebugUtilsMessengerEXT function = reinterpret_cast< PFN_vkCreateDebugUtilsMessengerEXT >( vkGetInstanceProcAddr( m_instance, "vkCreateDebugUtilsMessengerEXT" ) );
        if( !function )
            return function( m_instance, &create_info, nullptr, &m_debug_messenger );

        DF_LOG_WARNING( "Failed to create debug messenger" );
        return VK_ERROR_EXTENSION_NOT_PRESENT;
#endif
    }

    VkResult cVulkanRenderer::destroyDebugMessenger() const
    {
#ifdef DEBUG
        const PFN_vkDestroyDebugUtilsMessengerEXT function = reinterpret_cast< PFN_vkDestroyDebugUtilsMessengerEXT >( vkGetInstanceProcAddr( m_instance, "vkDestroyDebugUtilsMessengerEXT" ) );
        if( !function )
        {
            function( m_instance, m_debug_messenger, nullptr );
            return VK_SUCCESS;
        }

        DF_LOG_WARNING( "Failed to destroy debug messenger" );
        return VK_ERROR_EXTENSION_NOT_PRESENT;
#endif
    }

    VkBool32 cVulkanRenderer::debugMessageCallback( const VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
                                                    const VkDebugUtilsMessageTypeFlagsEXT        _message_type,
                                                    const VkDebugUtilsMessengerCallbackDataEXT*  _callback_data,
                                                    void* /*_user_data*/ )
    {
        ZoneScoped;

        std::string type = "None";
        if( _message_type >= VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT )
            type = "Performance";
        else if( _message_type >= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT )
            type = "Validation";
        else if( _message_type >= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT )
            type = "General";

        if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT )
        {
            DF_LOG_WARNING( std::format(
                               "Vulkan\n"
                               "Type: {}\n"
                               "Severity: Error\n"
                               "Message: {}", type, _callback_data->pMessage ) );
        }
        else if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT )
        {
            DF_LOG_WARNING( std::format(
                               "Vulkan\n"
                               "Type: {}\n"
                               "Severity: Warning\n"
                               "Message: {}", type, _callback_data->pMessage ) );
        }
        else if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT )
        {
            DF_LOG_WARNING( std::format(
                               "Vulkan\n"
                               "Type: {}\n"
                               "Severity: Info\n"
                               "Message: {}", type, _callback_data->pMessage ) );
        }
        else if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT )
        {
            DF_LOG_WARNING( std::format(
                               "Vulkan\n"
                               "Type: {}\n"
                               "Severity: Verbose\n"
                               "Message: {}", type, _callback_data->pMessage ) );
        }
        else
        {
            DF_LOG_WARNING( std::format(
                               "Vulkan\n"
                               "Type: {}\n"
                               "Severity: None\n"
                               "Message: {}", type, _callback_data->pMessage ) );
        }

        return VK_FALSE;
    }
}
