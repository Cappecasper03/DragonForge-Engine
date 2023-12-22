#include "cApplication.h"

#include <stb_image.h>
#include <windows.h>
#include <freetype/freetype.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/filesystem/cFileSystem.h"
#include "core/managers/cCameraManager.h"
#include "core/managers/cEventManager.h"
#include "core/managers/cFontManager.h"
#include "core/managers/cInputManager.h"
#include "core/managers/cModelManager.h"
#include "core/managers/cQuadManager.h"
#include "core/managers/cRenderCallbackManager.h"
#include "core/misc/cTimer.h"
#include "core/profiling/Profiling.h"
#include "core/rendering/cRenderer.h"
#include "core/rendering/assets/cameras/cFreeFlightCamera.h"

cApplication::cApplication()
{
#if defined ( DEBUG )
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
#if defined ( DEBUG )
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

    df::profiling::printClear();
}

void cApplication::run()
{
#if defined ( DEBUG )
    PROFILING_SCOPE( __FUNCTION__ );
#endif

    df::cFreeFlightCamera flight_camera( "freeflight", 1, .1f );

    df::cModel model( "test", "data/models/survival-guitar-backpack" );

    df::cRenderer::resizeWindow();
    df::cRenderer::setCursorInputMode( GLFW_CURSOR_DISABLED );

    while( !glfwWindowShouldClose( df::cRenderer::getWindow() ) )
    {
#if defined ( DEBUG )
        PROFILING_SCOPE( __FUNCTION__"::loop" );
#endif

        df::cInputManager::update();
        // df::cEventManager::invoke( df::event::update, m_timer.getDeltaSecond() );

        if( glfwGetKey( df::cRenderer::getWindow(), GLFW_KEY_ESCAPE ) == GLFW_PRESS )
            glfwSetWindowShouldClose( df::cRenderer::getWindow(), true );

        float delta_time = static_cast< float >( m_timer.getDeltaSecond() );
        flight_camera.update( delta_time );

        flight_camera.beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        model.render();
        flight_camera.endRender();

        df::cRenderer::render();
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

    df::filesystem::remove( "log.txt" );
    df::filesystem::remove( "memory.txt" );
    df::filesystem::remove( "profiling.txt" );

    LOG_RAW( "Starting DragonForge-Engine" );
}
