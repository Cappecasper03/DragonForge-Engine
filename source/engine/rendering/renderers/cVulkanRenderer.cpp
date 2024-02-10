#include "cVulkanRenderer.h"

#define GLFW_INCLUDE_VULKAN

#include <format>
#include <GLFW/glfw3.h>
#include <tracy/TracyVulkan.hpp>

#include "framework/application/cApplication.h"

namespace df
{
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
    }

    cVulkanRenderer::~cVulkanRenderer()
    {
        ZoneScoped;

        glfwDestroyWindow( m_window );

        m_glfw_use_count--;
        if( m_glfw_use_count == 0 )
        {
            glfwTerminate();
            DF_LOG_MESSAGE( "Deinitialized GLFW" );
        }
    }
}
