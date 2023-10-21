#include "cApplication.h"

#include <windows.h>
#include <GLFW/glfw3.h>

#include "core/filesystem/cFileSystem.h"
#include "core/managers/cEventManager.h"
#include "core/managers/cInputManager.h"
#include "core/misc/cTimer.h"
#include "core/profiling/Profiling.h"
#include "core/rendering/cRenderer.h"

cApplication::cApplication()
{
#if defined ( DEBUG )
    PROFILING_SCOPE( __FUNCTION__ );
#endif
    
    initialize();

    const df::cRenderer* renderer = df::cRenderer::initialize();
    df::cEventManager::initialize();
    df::cInputManager::initialize( renderer->getWindow() );
}

cApplication::~cApplication()
{
#if defined ( DEBUG )
    PROFILING_BEGIN( __FUNCTION__ );
#endif
    
    df::cInputManager::deinitialize();
    df::cEventManager::deinitialize();
    df::cRenderer::deinitialize();

#if defined ( DEBUG )
    PROFILING_END;
#endif
    
    df::profiling::printClear();
    df::memory::printLeaks();
}

void cApplication::run()
{
#if defined ( DEBUG )
    PROFILING_SCOPE( __FUNCTION__ );
#endif
    
    const df::cRenderer* renderer      = df::cRenderer::getInstance();
    df::cInputManager*   input_manager = df::cInputManager::getInstance();

    renderer->resizeWindow();
    while( !glfwWindowShouldClose( renderer->getWindow() ) )
    {
        input_manager->update();
        df::cEventManager::invoke( df::event::update, m_timer.getDeltaSecond() );
        renderer->render();
    }
}

void cApplication::initialize()
{
#if defined( DEBUG )
    PROFILING_SCOPE( __FUNCTION__ );

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
}
