#include "cRenderer.h"

#include <format>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application/cApplication.h"
#include "core/managers/cEventManager.h"

namespace df
{
    cRenderer::cRenderer()
    : m_window( nullptr )
    {
        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        LOG_MESSAGE( "Initialized GLFW" );

        constexpr int window_width  = 1200;
        constexpr int window_height = 800;
        m_window                    = glfwCreateWindow( window_width, window_height, cApplication::getInstance()->getName().c_str(), nullptr, nullptr );
        if( !m_window )
        {
            LOG_ERROR( "Failed to create window" );
            return;
        }
        LOG_MESSAGE( std::format("Created window [{}, {}]" , window_width, window_height ) );

        glfwMakeContextCurrent( m_window );

        if( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( glfwGetProcAddress ) ) )
        {
            LOG_ERROR( "Failed to initialize GLAD" );
            return;
        }
        LOG_MESSAGE( "Initialized GLAD" );

        glViewport( 0, 0, window_width, window_height );

        glfwSetFramebufferSizeCallback( m_window, framebufferSizeCallback );
    }

    cRenderer::~cRenderer()
    {
        glfwTerminate();
        LOG_MESSAGE( "Deinitialized GLFW" );
    }

    void cRenderer::render() const
    {
        cEventManager::invoke( event::render_3d );
        cEventManager::invoke( event::render_2d );
        glfwSwapBuffers( m_window );
    }

    void cRenderer::resizeWindow( const int& _width, const int& _height ) const
    {
        if( _width > 0 && _height > 0 )
        {
            glfwSetWindowSize( m_window, _width, _height );
            return;
        }

        int window_width, window_height;
        glfwGetWindowSize( m_window, &window_width, &window_height );

        cEventManager::invoke( event::on_window_resize, window_width, window_height );
    }

    void cRenderer::setCursorInputMode( const int& _value ) const
    {
        glfwSetInputMode( m_window, GLFW_CURSOR, _value );
    }

    void cRenderer::framebufferSizeCallback( GLFWwindow* /*_window*/, const int _width, const int _height )
    {
        glViewport( 0, 0, _width, _height );

        cEventManager::invoke( event::on_window_resize, _width, _height );
    }
}
