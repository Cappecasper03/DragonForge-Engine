#include "cApplication.h"

#include <format>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/filesystem/cFileSystem.h"
#include "core/profiling/Profiling.h"

cApplication::cApplication()
{
    size_t  size;
    wchar_t wbuffer[ MAX_PATH ];
    char    buffer[ MAX_PATH ];

    GetModuleFileName( nullptr, wbuffer, MAX_PATH );
    wcstombs_s( &size, buffer, MAX_PATH, wbuffer, MAX_PATH );

    const std::string executable_path( buffer );
    df::filesystem::setExecutableDirectory( executable_path.substr( 0, executable_path.find_last_of( '\\' ) + 1 ) );

#if defined( DEBUG )
    AllocConsole();
    FILE* file;
    freopen_s( &file, "CONOUT$", "w", stdout );
#endif

    df::filesystem::remove( "logs.txt" );
    df::filesystem::remove( "memory.txt" );
    df::filesystem::remove( "profiling.txt" );

    {
        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        LOG_MESSAGE( "Initialized GLFW" );

        std::string executable_name( executable_path.substr( executable_path.find_last_of( '\\' ) + 1 ) );
        executable_name.erase( executable_name.length() - 4 );

        constexpr int window_width  = 1200;
        constexpr int window_height = 800;
        m_window                    = glfwCreateWindow( window_width, window_height, executable_name.c_str(), nullptr, nullptr );
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

        glfwSetFramebufferSizeCallback( m_window, onResize );
    }
}

cApplication::~cApplication()
{
    df::profiling::printClear();
    df::memory::printLeaks();

    glfwTerminate();
}

void cApplication::run()
{
    while( !glfwWindowShouldClose( m_window ) )
    {
        input();
        update();
        render3D();
        render2D();
    }
}

void cApplication::onResize( GLFWwindow* /*_window*/, const int _width, const int _height )
{
    glViewport( 0, 0, _width, _height );
}

void cApplication::input()
{
    // TODO: Send input event to listeners
}

void cApplication::update()
{
    glfwSwapBuffers( m_window );
    glfwPollEvents();
}

void cApplication::render3D()
{}

void cApplication::render2D()
{}
