#include "cRenderer.h"

#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32

#include <format>
#include <map>
#include <set>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "engine/filesystem/cFileSystem.h"
#include "framework/application/cApplication.h"

namespace df::vulkan
{
    std::vector< const char* > cRenderer::validation_layers  = { "VK_LAYER_KHRONOS_validation" };
    std::vector< const char* > cRenderer::device_extenstions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    cRenderer::cRenderer()
    : m_window( nullptr ),
      m_instance( nullptr ),
      m_current_frame( 0 )
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

        if( glfwCreateWindowSurface( m_instance, m_window, nullptr, &m_surface ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to create surface" );
            return;
        }
        DF_LOG_MESSAGE( "Created window surface" );

        if( !pickPhysicalDevice() || !createLogicalDevice() )
            return;

        if( !createSwapChain() || !createImageViews() )
            return;

        if( !createRenderPass() || !createGraphicsPipeline() || !createFramebuffers() )
            return;

        if( !createCommandPool() || !createCommandBuffers() )
            return;

        if( !createSyncObjects() )
            return;

        DF_LOG_MESSAGE( "Initialized renderer" );
    }

    cRenderer::~cRenderer()
    {
        ZoneScoped;

        vkDeviceWaitIdle( m_logical_device );

        for( const VkSemaphore& semaphore : m_render_finish_semaphores )
            vkDestroySemaphore( m_logical_device, semaphore, nullptr );

        for( const VkSemaphore& semaphore : m_image_available_semaphores )
            vkDestroySemaphore( m_logical_device, semaphore, nullptr );

        for( const VkFence& fence : m_rendering_fences )
            vkDestroyFence( m_logical_device, fence, nullptr );

        vkDestroyCommandPool( m_logical_device, m_command_pool, nullptr );

        for( const VkFramebuffer& framebuffer : m_swap_chain_framebuffers )
            vkDestroyFramebuffer( m_logical_device, framebuffer, nullptr );

        vkDestroyPipeline( m_logical_device, m_pipeline, nullptr );

        vkDestroyPipelineLayout( m_logical_device, m_pipeline_layout, nullptr );

        vkDestroyRenderPass( m_logical_device, m_render_pass, nullptr );

        for( const VkImageView& image_view : m_swap_chain_image_views )
            vkDestroyImageView( m_logical_device, image_view, nullptr );

        vkDestroySwapchainKHR( m_logical_device, m_swap_chain, nullptr );

        vkDestroySurfaceKHR( m_instance, m_surface, nullptr );

        vkDestroyDevice( m_logical_device, nullptr );

#ifdef DEBUG
        destroyDebugMessenger();
#endif

        vkDestroyInstance( m_instance, nullptr );

        glfwDestroyWindow( m_window );

        m_glfw_use_count--;
        if( m_glfw_use_count == 0 )
        {
            glfwTerminate();
            DF_LOG_MESSAGE( "Deinitialized GLFW" );
        }

        DF_LOG_MESSAGE( "Deinitialized renderer" );
    }

    void cRenderer::render()
    {
        ZoneScoped;

        vkWaitForFences( m_logical_device, 1, &m_rendering_fences[ m_current_frame ], VK_TRUE, UINT64_MAX );
        vkResetFences( m_logical_device, 1, &m_rendering_fences[ m_current_frame ] );

        uint32_t image_index;
        vkAcquireNextImageKHR( m_logical_device, m_swap_chain, UINT64_MAX, m_image_available_semaphores[ m_current_frame ], VK_NULL_HANDLE, &image_index );

        vkResetCommandBuffer( m_command_buffers[ m_current_frame ], 0 );
        recordCommandBuffer( m_command_buffers[ m_current_frame ], image_index );

        const std::vector                         wait_semaphores   = { m_image_available_semaphores[ m_current_frame ] };
        const std::vector                         signal_semaphores = { m_render_finish_semaphores[ m_current_frame ] };
        const std::vector< VkPipelineStageFlags > wait_stages       = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submit_info{};
        submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount   = 1;
        submit_info.pCommandBuffers      = &m_command_buffers[ m_current_frame ];
        submit_info.waitSemaphoreCount   = static_cast< uint32_t >( wait_semaphores.size() );
        submit_info.pWaitSemaphores      = wait_semaphores.data();
        submit_info.pWaitDstStageMask    = wait_stages.data();
        submit_info.signalSemaphoreCount = static_cast< uint32_t >( signal_semaphores.size() );
        submit_info.pSignalSemaphores    = signal_semaphores.data();

        if( vkQueueSubmit( m_graphics_queue, 1, &submit_info, m_rendering_fences[ m_current_frame ] ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to submit render queue" );
            return;
        }

        const std::vector swap_chains = { m_swap_chain };

        VkPresentInfoKHR present_info{};
        present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.swapchainCount     = static_cast< uint32_t >( swap_chains.size() );
        present_info.pSwapchains        = swap_chains.data();
        present_info.waitSemaphoreCount = static_cast< uint32_t >( signal_semaphores.size() );
        present_info.pWaitSemaphores    = signal_semaphores.data();
        present_info.pImageIndices      = &image_index;
        present_info.pResults           = nullptr;

        vkQueuePresentKHR( m_present_queue, &present_info );
        m_current_frame = ++m_current_frame % max_frames_rendering;
    }

    std::vector< const char* > cRenderer::getRequiredExtensions()
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

    bool cRenderer::createInstance()
    {
        ZoneScoped;

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
        create_info.enabledLayerCount       = 0;

#ifdef DEBUG
        if( checkValidationLayers() )
        {
            create_info.enabledLayerCount   = static_cast< uint32_t >( validation_layers.size() );
            create_info.ppEnabledLayerNames = validation_layers.data();
        }

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

    bool cRenderer::createLogicalDevice()
    {
        ZoneScoped;

        const sQueueFamilyIndices indices        = findQueueFamilies( m_physical_device );
        constexpr float           queue_priority = 1;

        std::vector< VkDeviceQueueCreateInfo > queue_create_infos;

        for( const uint32_t family : std::set{ indices.graphics.value(), indices.present.value() } )
        {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = family;
            queue_create_info.queueCount       = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back( queue_create_info );
        }

        constexpr VkPhysicalDeviceFeatures device_features{};

        VkDeviceCreateInfo create_info{};
        create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos       = queue_create_infos.data();
        create_info.queueCreateInfoCount    = static_cast< uint32_t >( queue_create_infos.size() );
        create_info.pEnabledFeatures        = &device_features;
        create_info.enabledExtensionCount   = static_cast< uint32_t >( device_extenstions.size() );
        create_info.ppEnabledExtensionNames = device_extenstions.data();
        create_info.enabledLayerCount       = 0;

#ifdef DEBUG
        if( checkValidationLayers() )
        {
            create_info.enabledLayerCount   = static_cast< uint32_t >( validation_layers.size() );
            create_info.ppEnabledLayerNames = validation_layers.data();
        }
#endif

        if( vkCreateDevice( m_physical_device, &create_info, nullptr, &m_logical_device ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to create logical device" );
            return false;
        }

        vkGetDeviceQueue( m_logical_device, indices.graphics.value(), 0, &m_graphics_queue );
        vkGetDeviceQueue( m_logical_device, indices.present.value(), 0, &m_present_queue );

        DF_LOG_MESSAGE( "Created logical device" );
        return true;
    }

    bool cRenderer::createSwapChain()
    {
        ZoneScoped;

        const sSwapChainSupportDetails swap_chain_support = querySwapChainSupport( m_physical_device );

        const VkSurfaceFormatKHR surface_format = chooseSwapChainSurfaceFormat( swap_chain_support.formats );
        const VkPresentModeKHR   present_mode   = chooseSwapChainPresentMode( swap_chain_support.present_modes );
        const VkExtent2D         extent         = chooseSwapChainExtent( swap_chain_support.capabilities );

        uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;
        if( swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount )
            image_count = swap_chain_support.capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface          = m_surface;
        create_info.minImageCount    = image_count;
        create_info.imageFormat      = surface_format.format;
        create_info.imageColorSpace  = surface_format.colorSpace;
        create_info.imageExtent      = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        create_info.preTransform     = swap_chain_support.capabilities.currentTransform;
        create_info.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode      = present_mode;
        create_info.clipped          = VK_TRUE;
        create_info.oldSwapchain     = VK_NULL_HANDLE;

        const sQueueFamilyIndices indices                 = findQueueFamilies( m_physical_device );
        const uint32_t            queue_family_indices[ ] = { indices.graphics.value(), indices.present.value() };

        if( indices.graphics != indices.present )
        {
            create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices   = queue_family_indices;
        }
        else
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if( vkCreateSwapchainKHR( m_logical_device, &create_info, nullptr, &m_swap_chain ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to create swap chain" );
            return false;
        }

        vkGetSwapchainImagesKHR( m_logical_device, m_swap_chain, &image_count, nullptr );
        m_swap_chain_images.resize( image_count );
        vkGetSwapchainImagesKHR( m_logical_device, m_swap_chain, &image_count, m_swap_chain_images.data() );
        m_swap_chain_format = surface_format.format;
        m_swap_chain_extent = extent;

        DF_LOG_MESSAGE( "Created swap chain" );
        return true;
    }

    bool cRenderer::createImageViews()
    {
        ZoneScoped;

        m_swap_chain_image_views.resize( m_swap_chain_images.size() );

        for( size_t i = 0; i < m_swap_chain_image_views.size(); ++i )
        {
            VkImageViewCreateInfo create_info{};
            create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image                           = m_swap_chain_images[ i ];
            create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format                          = m_swap_chain_format;
            create_info.components                      = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
            create_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel   = 0;
            create_info.subresourceRange.levelCount     = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount     = 1;

            if( vkCreateImageView( m_logical_device, &create_info, nullptr, &m_swap_chain_image_views[ i ] ) != VK_SUCCESS )
            {
                DF_LOG_ERROR( "Failed to create swap chain image view" );
                return false;
            }
        }

        DF_LOG_MESSAGE( "Created swap chain image views" );
        return true;
    }

    bool cRenderer::createRenderPass()
    {
        ZoneScoped;

        VkAttachmentDescription color_attachment{};
        color_attachment.format         = m_swap_chain_format;
        color_attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_reference{};
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments    = &color_attachment_reference;

        VkSubpassDependency dependency{};
        dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass    = 0;
        dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo create_info{};
        create_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        create_info.attachmentCount = 1;
        create_info.pAttachments    = &color_attachment;
        create_info.subpassCount    = 1;
        create_info.pSubpasses      = &subpass;
        create_info.dependencyCount = 1;
        create_info.pDependencies   = &dependency;

        if( vkCreateRenderPass( m_logical_device, &create_info, nullptr, &m_render_pass ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to create render pass" );
            return false;
        }

        DF_LOG_MESSAGE( "Created render pass" );
        return true;
    }

    bool cRenderer::createGraphicsPipeline()
    {
        ZoneScoped;

        const std::vector< char > vertex_shader   = loadShader( "vertex.spv" );
        const std::vector< char > fragment_shader = loadShader( "fragment.spv" );

        if( vertex_shader.empty() || fragment_shader.empty() )
        {
            DF_LOG_ERROR( "Failed to create graphics pipeline" );
            return false;
        }

        const VkShaderModule vertex_module   = createShaderModule( vertex_shader );
        const VkShaderModule fragment_module = createShaderModule( fragment_shader );

        std::vector< VkPipelineShaderStageCreateInfo > shader_stages_create_info( 2 );
        shader_stages_create_info[ 0 ].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages_create_info[ 0 ].stage  = VK_SHADER_STAGE_VERTEX_BIT;
        shader_stages_create_info[ 0 ].module = vertex_module;
        shader_stages_create_info[ 0 ].pName  = "main";

        shader_stages_create_info[ 1 ].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages_create_info[ 1 ].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        shader_stages_create_info[ 1 ].module = fragment_module;
        shader_stages_create_info[ 1 ].pName  = "main";

        const std::vector                dynamic_states = { VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT };
        VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
        dynamic_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.dynamicStateCount = static_cast< uint32_t >( dynamic_states.size() );
        dynamic_state_create_info.pDynamicStates    = dynamic_states.data();

        VkPipelineVertexInputStateCreateInfo vertex_input_create_info{};
        vertex_input_create_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_create_info.vertexBindingDescriptionCount   = 0;
        vertex_input_create_info.vertexAttributeDescriptionCount = 0;

        VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info{};
        input_assembly_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewport_state_create_info{};
        viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_create_info.viewportCount = 1;
        viewport_state_create_info.scissorCount  = 1;

        VkPipelineRasterizationStateCreateInfo rasterization_create_info{};
        rasterization_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization_create_info.depthClampEnable        = VK_FALSE;
        rasterization_create_info.rasterizerDiscardEnable = VK_FALSE;
        rasterization_create_info.polygonMode             = VK_POLYGON_MODE_FILL;
        rasterization_create_info.lineWidth               = 1;
        rasterization_create_info.cullMode                = VK_CULL_MODE_BACK_BIT;
        rasterization_create_info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
        rasterization_create_info.depthBiasEnable         = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisample_create_info{};
        multisample_create_info.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisample_create_info.sampleShadingEnable  = VK_FALSE;
        multisample_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment.blendEnable    = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo color_blend_create_info{};
        color_blend_create_info.sType               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_create_info.logicOpEnable       = VK_FALSE;
        color_blend_create_info.logicOp             = VK_LOGIC_OP_COPY;
        color_blend_create_info.attachmentCount     = 1;
        color_blend_create_info.pAttachments        = &color_blend_attachment;
        color_blend_create_info.blendConstants[ 0 ] = 0;
        color_blend_create_info.blendConstants[ 1 ] = 0;
        color_blend_create_info.blendConstants[ 2 ] = 0;
        color_blend_create_info.blendConstants[ 3 ] = 0;

        VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
        pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        if( vkCreatePipelineLayout( m_logical_device, &pipeline_layout_create_info, nullptr, &m_pipeline_layout ) != VK_SUCCESS )
            return false;

        VkGraphicsPipelineCreateInfo create_info{};
        create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        create_info.stageCount          = static_cast< uint32_t >( shader_stages_create_info.size() );
        create_info.pStages             = shader_stages_create_info.data();
        create_info.pVertexInputState   = &vertex_input_create_info;
        create_info.pInputAssemblyState = &input_assembly_create_info;
        create_info.pViewportState      = &viewport_state_create_info;
        create_info.pRasterizationState = &rasterization_create_info;
        create_info.pMultisampleState   = &multisample_create_info;
        create_info.pColorBlendState    = &color_blend_create_info;
        create_info.pDynamicState       = &dynamic_state_create_info;
        create_info.layout              = m_pipeline_layout;
        create_info.renderPass          = m_render_pass;
        create_info.subpass             = 0;
        create_info.basePipelineHandle  = nullptr;

        if( vkCreateGraphicsPipelines( m_logical_device, VK_NULL_HANDLE, 1, &create_info, nullptr, &m_pipeline ) == VK_SUCCESS )
            DF_LOG_MESSAGE( "Created graphics pipeline" );
        else
            DF_LOG_ERROR( "Failed to create graphics pipeline" );

        vkDestroyShaderModule( m_logical_device, fragment_module, nullptr );
        vkDestroyShaderModule( m_logical_device, vertex_module, nullptr );
        return true;
    }

    bool cRenderer::createFramebuffers()
    {
        ZoneScoped;

        m_swap_chain_framebuffers.resize( m_swap_chain_image_views.size() );

        for( size_t i = 0; i < m_swap_chain_framebuffers.size(); ++i )
        {
            const std::vector attachments = { m_swap_chain_image_views[ i ] };

            VkFramebufferCreateInfo create_info{};
            create_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            create_info.renderPass      = m_render_pass;
            create_info.attachmentCount = static_cast< uint32_t >( attachments.size() );
            create_info.pAttachments    = attachments.data();
            create_info.width           = m_swap_chain_extent.width;
            create_info.height          = m_swap_chain_extent.height;
            create_info.layers          = 1;

            if( vkCreateFramebuffer( m_logical_device, &create_info, nullptr, &m_swap_chain_framebuffers[ i ] ) != VK_SUCCESS )
            {
                DF_LOG_ERROR( "Failed to create framebuffer" );
                return false;
            }
        }

        DF_LOG_MESSAGE( "Created framebuffers" );
        return true;
    }

    bool cRenderer::createCommandPool()
    {
        ZoneScoped;

        const sQueueFamilyIndices indices = findQueueFamilies( m_physical_device );

        VkCommandPoolCreateInfo create_info{};
        create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        create_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        create_info.queueFamilyIndex = indices.graphics.value();

        if( vkCreateCommandPool( m_logical_device, &create_info, nullptr, &m_command_pool ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to create command pool" );
            return false;
        }

        DF_LOG_MESSAGE( "Created command pool" );
        return true;
    }

    bool cRenderer::createCommandBuffers()
    {
        ZoneScoped;

        m_command_buffers.resize( max_frames_rendering );

        VkCommandBufferAllocateInfo allocate_info{};
        allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocate_info.commandPool        = m_command_pool;
        allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocate_info.commandBufferCount = static_cast< uint32_t >( m_command_buffers.size() );

        if( vkAllocateCommandBuffers( m_logical_device, &allocate_info, m_command_buffers.data() ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to allocate command buffer" );
            return false;
        }

        DF_LOG_MESSAGE( "Created command buffer" );
        return true;
    }

    bool cRenderer::createSyncObjects()
    {
        ZoneScoped;

        m_image_available_semaphores.resize( max_frames_rendering );
        m_render_finish_semaphores.resize( max_frames_rendering );
        m_rendering_fences.resize( max_frames_rendering );

        VkSemaphoreCreateInfo semaphore_create_info{};
        semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_create_info{};
        fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for( int i = 0; i < max_frames_rendering; ++i )
        {
            if( vkCreateSemaphore( m_logical_device, &semaphore_create_info, nullptr, &m_image_available_semaphores[ i ] ) != VK_SUCCESS ||
                vkCreateSemaphore( m_logical_device, &semaphore_create_info, nullptr, &m_render_finish_semaphores[ i ] ) != VK_SUCCESS ||
                vkCreateFence( m_logical_device, &fence_create_info, nullptr, &m_rendering_fences[ i ] ) != VK_SUCCESS )
            {
                DF_LOG_ERROR( "Failed to create sync objects" );
                return false;
            }
        }

        DF_LOG_MESSAGE( "Created sync objects" );
        return true;
    }

    void cRenderer::recordCommandBuffer( const VkCommandBuffer _buffer, uint32_t _image_index ) const
    {
        ZoneScoped;

        VkCommandBufferBeginInfo command_buffer_begin_info{};
        command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.flags            = 0;
        command_buffer_begin_info.pInheritanceInfo = nullptr;

        if( vkBeginCommandBuffer( _buffer, &command_buffer_begin_info ) != VK_SUCCESS )
        {
            DF_LOG_ERROR( "Failed to begin command buffer" );
            return;
        }

        constexpr VkClearValue clear_color = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };

        VkRenderPassBeginInfo render_pass_begin_info{};
        render_pass_begin_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_begin_info.renderPass        = m_render_pass;
        render_pass_begin_info.framebuffer       = m_swap_chain_framebuffers[ _image_index ];
        render_pass_begin_info.renderArea.offset = { 0, 0 };
        render_pass_begin_info.renderArea.extent = m_swap_chain_extent;
        render_pass_begin_info.clearValueCount   = 1;
        render_pass_begin_info.pClearValues      = &clear_color;

        vkCmdBeginRenderPass( _buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE );
        vkCmdBindPipeline( _buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline );

        VkViewport viewport{};
        viewport.x        = 0;
        viewport.y        = 0;
        viewport.width    = static_cast< float >( m_swap_chain_extent.width );
        viewport.height   = static_cast< float >( m_swap_chain_extent.height );
        viewport.minDepth = 0;
        viewport.maxDepth = 1;
        vkCmdSetViewport( _buffer, 0, 1, &viewport );

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = m_swap_chain_extent;
        vkCmdSetScissor( _buffer, 0, 1, &scissor );

        vkCmdDraw( _buffer, 3, 1, 0, 0 );
        vkCmdEndRenderPass( _buffer );

        if( vkEndCommandBuffer( _buffer ) != VK_SUCCESS )
            DF_LOG_ERROR( "Failed to end command buffer" );
    }

    bool cRenderer::checkValidationLayers()
    {
        ZoneScoped;

        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties( &layer_count, nullptr );
        std::vector< VkLayerProperties > layers( layer_count );
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

    bool cRenderer::checkDeviceExtensions( const VkPhysicalDevice& _device )
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

    bool cRenderer::pickPhysicalDevice()
    {
        ZoneScoped;

        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices( m_instance, &device_count, nullptr );

        if( device_count == 0 )
        {
            DF_LOG_ERROR( "No GPUs with support for Vulkan" );
            return false;
        }

        std::vector< VkPhysicalDevice > devices( device_count );
        vkEnumeratePhysicalDevices( m_instance, &device_count, devices.data() );

        std::map< int, VkPhysicalDevice > rated_devices;
        for( const VkPhysicalDevice& device : devices )
            rated_devices.insert( std::make_pair( rateDeviceSuitability( device ), device ) );

        if( rated_devices.empty() || rated_devices.rbegin()->first <= 0 )
        {
            DF_LOG_ERROR( "Failed to find suitable GPU" );
            return false;
        }

        DF_LOG_MESSAGE( "Found suitable GPU" );
        m_physical_device = rated_devices.rbegin()->second;
        return true;
    }

    int cRenderer::rateDeviceSuitability( const VkPhysicalDevice& _device ) const
    {
        ZoneScoped;

        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties( _device, &device_properties );

        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceFeatures( _device, &device_features );

        if( !device_features.geometryShader || !findQueueFamilies( _device ).isComplete() || !checkDeviceExtensions( _device ) || !querySwapChainSupport( _device ).isSupported() )
            return 0;

        int score = 0;

        if( device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
            score += 10;

        score += device_properties.limits.maxImageDimension2D;

        return score;
    }

    cRenderer::sQueueFamilyIndices cRenderer::findQueueFamilies( const VkPhysicalDevice& _device ) const
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

    cRenderer::sSwapChainSupportDetails cRenderer::querySwapChainSupport( const VkPhysicalDevice& _device ) const
    {
        ZoneScoped;

        sSwapChainSupportDetails details{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR( _device, m_surface, &details.capabilities );

        uint32_t format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR( _device, m_surface, &format_count, nullptr );

        if( format_count != 0 )
        {
            details.formats.resize( format_count );
            vkGetPhysicalDeviceSurfaceFormatsKHR( _device, m_surface, &format_count, details.formats.data() );
        }

        uint32_t present_mode_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR( _device, m_surface, &present_mode_count, nullptr );

        if( present_mode_count != 0 )
        {
            details.present_modes.resize( present_mode_count );
            vkGetPhysicalDeviceSurfacePresentModesKHR( _device, m_surface, &present_mode_count, details.present_modes.data() );
        }

        return details;
    }

    VkSurfaceFormatKHR cRenderer::chooseSwapChainSurfaceFormat( const std::vector< VkSurfaceFormatKHR >& _formats )
    {
        ZoneScoped;

        for( const VkSurfaceFormatKHR& available_format : _formats )
        {
            if( available_format.format == VK_FORMAT_B8G8R8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
                return available_format;
        }

        return _formats.front();
    }

    VkPresentModeKHR cRenderer::chooseSwapChainPresentMode( const std::vector< VkPresentModeKHR >& _present_modes )
    {
        ZoneScoped;

        for( const VkPresentModeKHR& available_mode : _present_modes )
        {
            if( available_mode == VK_PRESENT_MODE_MAILBOX_KHR )
                return available_mode;
        }

        return _present_modes.front();
    }

    VkExtent2D cRenderer::chooseSwapChainExtent( const VkSurfaceCapabilitiesKHR& _capabilities ) const
    {
        ZoneScoped;

        if( _capabilities.currentExtent.width != UINT32_MAX )
            return _capabilities.currentExtent;

        int width, height;
        glfwGetFramebufferSize( m_window, &width, &height );

        VkExtent2D extent = { static_cast< uint32_t >( width ), static_cast< uint32_t >( height ) };
        extent.width      = std::clamp( extent.width, _capabilities.minImageExtent.width, _capabilities.maxImageExtent.width );
        extent.height     = std::clamp( extent.height, _capabilities.minImageExtent.height, _capabilities.maxImageExtent.height );

        return extent;
    }

    std::vector< char > cRenderer::loadShader( const std::string& _name )
    {
        ZoneScoped;

        std::vector< char > shader;

        std::fstream shader_file = filesystem::open( "binaries/shaders/" + _name, std::ios::in | std::ios::ate | std::ios::binary );
        if( !shader_file.is_open() )
        {
            DF_LOG_ERROR( std::format( "Failed to load shader: {}", _name ) );
            return shader;
        }

        shader.resize( shader_file.tellg() );
        shader_file.seekg( 0 );
        shader_file.read( shader.data(), shader.size() );

        DF_LOG_MESSAGE( std::format( "Successfully loaded shader: {}", _name ) );
        return shader;
    }

    VkShaderModule cRenderer::createShaderModule( const std::vector< char >& _shader ) const
    {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = _shader.size();
        create_info.pCode    = reinterpret_cast< const uint32_t* >( _shader.data() );

        VkShaderModule module;
        if( vkCreateShaderModule( m_logical_device, &create_info, nullptr, &module ) != VK_SUCCESS )
            DF_LOG_ERROR( "Failed to create shader module" );

        return module;
    }

    VkResult cRenderer::createDebugMessenger()
    {
#ifdef DEBUG
        ZoneScoped;

        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
        create_info.pfnUserCallback = debugMessageCallback;
        create_info.pUserData       = this;

        const PFN_vkCreateDebugUtilsMessengerEXT function = reinterpret_cast< PFN_vkCreateDebugUtilsMessengerEXT >( vkGetInstanceProcAddr( m_instance, "vkCreateDebugUtilsMessengerEXT" ) );
        if( function )
            return function( m_instance, &create_info, nullptr, &m_debug_messenger );

        DF_LOG_WARNING( "Failed to create debug messenger" );
        return VK_ERROR_EXTENSION_NOT_PRESENT;
#else
        return VK_SUCCESS;
#endif
    }

    VkResult cRenderer::destroyDebugMessenger() const
    {
#ifdef DEBUG
        ZoneScoped;

        const PFN_vkDestroyDebugUtilsMessengerEXT function = reinterpret_cast< PFN_vkDestroyDebugUtilsMessengerEXT >( vkGetInstanceProcAddr( m_instance, "vkDestroyDebugUtilsMessengerEXT" ) );
        if( function )
        {
            function( m_instance, m_debug_messenger, nullptr );
            return VK_SUCCESS;
        }

        DF_LOG_WARNING( "Failed to destroy debug messenger" );
        return VK_ERROR_EXTENSION_NOT_PRESENT;
#else
        return VK_SUCCESS;
#endif
    }

    VkBool32 cRenderer::debugMessageCallback( const VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
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
            DF_LOG_ERROR( std::format(
                             "Vulkan, "
                             "Type: {}, "
                             "Severity: Error, "
                             "Message: {}", type, _callback_data->pMessage ) );
        }
        else if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT )
        {
            DF_LOG_WARNING( std::format(
                               "Vulkan, "
                               "Type: {}, "
                               "Severity: Warning, "
                               "Message: {}", type, _callback_data->pMessage ) );
        }
        else if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT )
        {
            DF_LOG_MESSAGE( std::format(
                               "Vulkan, "
                               "Type: {}, "
                               "Severity: Info, "
                               "Message: {}", type, _callback_data->pMessage ) );
        }
        else if( _message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT )
        {
            DF_LOG_MESSAGE( std::format(
                               "Vulkan, "
                               "Type: {}, "
                               "Severity: Verbose, "
                               "Message: {}", type, _callback_data->pMessage ) );
        }
        else
        {
            DF_LOG_MESSAGE( std::format(
                               "Vulkan, "
                               "Type: {}, "
                               "Severity: None, "
                               "Message: {}", type, _callback_data->pMessage ) );
        }

        return VK_FALSE;
    }
}
