#include "cRenderer.h"

#include <format>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application/cApplication.h"
#include "core/managers/cEventManager.h"

namespace df
{
    cRenderer::cRenderer()
    : m_window( nullptr ),
      m_window_size( 1200, 800 )
    {
        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        LOG_MESSAGE( "Initialized GLFW" );

        m_window = glfwCreateWindow( m_window_size.x, m_window_size.y, cApplication::getName().c_str(), nullptr, nullptr );
        if( !m_window )
        {
            LOG_ERROR( "Failed to create window" );
            return;
        }
        LOG_MESSAGE( std::format("Created window [{}, {}]" , m_window_size.x, m_window_size.y ) );

        glfwMakeContextCurrent( m_window );

        if( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( glfwGetProcAddress ) ) )
        {
            LOG_ERROR( "Failed to initialize GLAD" );
            return;
        }
        LOG_MESSAGE( "Initialized GLAD" );

        glViewport( 0, 0, m_window_size.x, m_window_size.y );

        glfwSetFramebufferSizeCallback( m_window, framebufferSizeCallback );

#if defined ( DEBUG )
        glEnable( GL_DEBUG_OUTPUT );
        glDebugMessageCallback( debugMessageCallback, nullptr );
#endif
    }

    cRenderer::~cRenderer()
    {
        glfwTerminate();
        LOG_MESSAGE( "Deinitialized GLFW" );
    }

    void cRenderer::render()
    {
        cEventManager::invoke( event::render_3d );
        cEventManager::invoke( event::render_2d );

        glfwSwapBuffers( getInstance()->m_window );
    }

    void cRenderer::resizeWindow( const int& _width, const int& _height )
    {
        const cRenderer* renderer = getInstance();

        if( _width > 0 && _height > 0 )
        {
            glfwSetWindowSize( renderer->m_window, _width, _height );
            return;
        }

        cEventManager::invoke( event::on_window_resize, renderer->m_window_size.x, renderer->m_window_size.y );
    }

    void cRenderer::setCursorInputMode( const int& _value )
    {
        glfwSetInputMode( getInstance()->m_window, GLFW_CURSOR, _value );
    }

    void cRenderer::framebufferSizeCallback( GLFWwindow* /*_window*/, const int _width, const int _height )
    {
        glViewport( 0, 0, _width, _height );

        if( _width == 0 || _height == 0 )
            return;

        getInstance()->m_window_size = { _width, _height };
        cEventManager::invoke( event::on_window_resize, _width, _height );
    }

    void cRenderer::debugMessageCallback( unsigned _source, unsigned _type, unsigned _id, unsigned _severity, int /*_length*/, const char* _message, const void* /*_user_param*/ )
    {
        std::string source;
        switch( _source )
        {
            case GL_DEBUG_SOURCE_API: { source = "API"; }
            break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: { source = "Window System"; }
            break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: { source = "Shader Compiler"; }
            break;
            case GL_DEBUG_SOURCE_THIRD_PARTY: { source = "Third Party"; }
            break;
            case GL_DEBUG_SOURCE_APPLICATION: { source = "Application"; }
            break;
            case GL_DEBUG_SOURCE_OTHER: { source = "Other"; }
            break;
        }

        std::string type;
        switch( _type )
        {
            case GL_DEBUG_TYPE_ERROR: { type = "Error"; }
            break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: { type = "Deprecated Behavior"; }
            break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: { type = "Undefined Behavior"; }
            break;
            case GL_DEBUG_TYPE_PORTABILITY: { type = "Portability"; }
            break;
            case GL_DEBUG_TYPE_PERFORMANCE: { type = "Performance"; }
            break;
            case GL_DEBUG_TYPE_MARKER: { type = "Marker"; }
            break;
            case GL_DEBUG_TYPE_PUSH_GROUP: { type = "Push Group"; }
            break;
            case GL_DEBUG_TYPE_POP_GROUP: { type = "Pop Group"; }
            break;
            case GL_DEBUG_TYPE_OTHER: { type = "Other"; }
            break;
        }

        switch( _severity )
        {
            case GL_DEBUG_SEVERITY_HIGH:
            {
                LOG_ERROR( std::format(
                              "OpenGL\n"
                              "Source: {}\n"
                              "Type: {}\n"
                              "ID: {}\n"
                              "Severity: High\n"
                              "Message: {}", source, type, _id, _message ) );
            }
            break;
            case GL_DEBUG_SEVERITY_MEDIUM:
            {
                LOG_WARNING( std::format(
                                "OpenGL\n"
                                "Source: {}\n"
                                "Type: {}\n"
                                "ID: {}\n"
                                "Severity: Medium\n"
                                "Message: {}", source, type, _id, _message ) );
            }
            break;
            case GL_DEBUG_SEVERITY_LOW:
            {
                LOG_WARNING( std::format(
                                "OpenGL\n"
                                "Source: {}\n"
                                "Type: {}\n"
                                "ID: {}\n"
                                "Severity: Low\n"
                                "Message: {}", source, type, _id, _message ) );
            }
            break;
        }
    }
}
