#include "cApplication.h"

#include <filesystem>
#include <windows.h>
#include <freetype/freetype.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "cTesting.hpp"
#include "core/filesystem/cFileSystem.h"
#include "core/managers/cEventManager.h"
#include "core/managers/cInputManager.h"
#include "core/managers/cRenderCallbackManager.h"
#include "core/managers/assets/cCameraManager.h"
#include "core/managers/assets/cFontManager.h"
#include "core/managers/assets/cModelManager.h"
#include "core/managers/assets/cQuadManager.h"
#include "core/misc/cTimer.h"
#include "core/rendering/cRenderer.h"

#if PROFILING
#include "core/profiling/Profiling.h"
#endif

cApplication::cApplication()
{
#if PROFILING
    PROFILING_SCOPE( __FUNCTION__ );
#endif

    initialize();

    df::cRenderer::initialize();
    df::cEventManager::initialize();
    df::cInputManager::initialize();
    df::cRenderCallbackManager::initialize();
    df::cQuadManager::initialize();
    df::cFontManager::initialize();
    df::cModelManager::initialize();
    df::cCameraManager::initialize();
}

cApplication::~cApplication()
{
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        df::cCameraManager::deinitialize();
        df::cModelManager::deinitialize();
        df::cFontManager::deinitialize();
        df::cQuadManager::deinitialize();
        df::cRenderCallbackManager::deinitialize();
        df::cInputManager::deinitialize();
        df::cEventManager::deinitialize();
        df::cRenderer::deinitialize();
    }

#if PROFILING
    df::profiling::printClear();
#endif
}

void cApplication::run()
{
#if PROFILING
    PROFILING_SCOPE( __FUNCTION__ );
#endif

    cTesting* testing = new cTesting();
    testing;

    df::cRenderer::resizeWindow();
    df::cRenderer::setCursorInputMode( GLFW_CURSOR_DISABLED );

    while( !glfwWindowShouldClose( df::cRenderer::getWindow() ) )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__"::loop" );
#endif

        df::cInputManager::update();
        df::cEventManager::invoke( df::event::update, static_cast< float >( m_timer.getDeltaSecond() ) );
        df::cRenderer::render();
    }
}

void cApplication::initialize()
{
#if PROFILING
    PROFILING_SCOPE( __FUNCTION__ );
#endif

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
    const std::filesystem::path executable_path( buffer );

    df::filesystem::setGameDirectory( executable_path.parent_path().parent_path().string() + "\\" );
    m_name = executable_path.filename().replace_extension().string();

    df::filesystem::remove( "log.txt" );
    df::filesystem::remove( "profiling.csv" );

    LOG_RAW( "Starting DragonForge-Engine" );
}
