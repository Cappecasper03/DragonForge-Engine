#include "cApplication.h"

#include <format>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/filesystem/cFileSystem.h"
#include "core/managers/cEventManager.h"
#include "core/managers/cInputManager.h"
#include "core/misc/cTimer.h"
#include "core/profiling/Profiling.h"

cApplication::cApplication()
: m_window( nullptr )
{
#if defined( DEBUG )
    AllocConsole();
    FILE* file;
    freopen_s( &file, "CONOUT$", "w", stdout );
    SetConsoleTitle( L"DragonForge-Engine Logs" );
#endif

    size_t  size;
    wchar_t wbuffer[ MAX_PATH ];
    char    buffer[ MAX_PATH ];

    GetModuleFileName( nullptr, wbuffer, MAX_PATH );
    wcstombs_s( &size, buffer, MAX_PATH, wbuffer, MAX_PATH );

    const std::string executable_path( buffer );
    df::filesystem::setExecutableDirectory( executable_path.substr( 0, executable_path.find_last_of( '\\' ) + 1 ) );

    m_name = executable_path.substr( executable_path.find_last_of( '\\' ) + 1 );
    m_name.erase( m_name.length() - 4 );

    df::filesystem::remove( "logs.txt" );
    df::filesystem::remove( "memory.txt" );
    df::filesystem::remove( "profiling.txt" );

    LOG_RAW( "Starting DragonForge-Engine" );

    initializeOpenGL();

    df::cEventManager::initialize();
    df::cInputManager::initialize( m_window );
}

cApplication::~cApplication()
{
    df::profiling::printClear();
    df::memory::printLeaks();

    df::cInputManager::deinitialize();
    df::cEventManager::deinitialize();

    glfwTerminate();
    LOG_MESSAGE( "Deinitialized GLFW" );
}

void cApplication::run()
{
    int window_width, window_height;
    glfwGetWindowSize( m_window, &window_width, &window_height );
    df::cEventManager::invoke( df::event::on_window_resize, window_width, window_height );

    while( !glfwWindowShouldClose( m_window ) )
    {
        df::cInputManager::getInstance()->update();
        df::cEventManager::invoke( df::event::update, m_timer.getDeltaSecond() );

        df::cEventManager::invoke( df::event::render_3d );
        df::cEventManager::invoke( df::event::render_2d );
        glfwSwapBuffers( m_window );
    }
}

void cApplication::framebufferSizeCallback( GLFWwindow* /*_window*/, const int _width, const int _height )
{
    glViewport( 0, 0, _width, _height );

    df::cEventManager::invoke( df::event::on_window_resize, _width, _height );
}

void cApplication::initializeOpenGL()
{
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    LOG_MESSAGE( "Initialized GLFW" );

    constexpr int window_width  = 1200;
    constexpr int window_height = 800;
    m_window                    = glfwCreateWindow( window_width, window_height, m_name.c_str(), nullptr, nullptr );
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
