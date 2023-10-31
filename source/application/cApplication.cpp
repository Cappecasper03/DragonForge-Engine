#include "cApplication.h"

#include <stb_image.h>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <freetype/freetype.h>

#include "core/filesystem/cFileSystem.h"
#include "core/managers/cCameraManager.h"
#include "core/managers/cEventManager.h"
#include "core/managers/cInputManager.h"
#include "core/managers/cQuadManager.h"
#include "core/managers/cRenderCallbackManager.h"
#include "core/misc/cTimer.h"
#include "core/profiling/Profiling.h"
#include "core/rendering/cRenderer.h"
#include "core/rendering/assets/cFont.h"
#include "core/rendering/assets/cQuad.h"
#include "core/rendering/cameras/cFreeFlightCamera.h"

cApplication::cApplication()
{
#if defined ( DEBUG )
    PROFILING_SCOPE( __FUNCTION__ );
#endif

    initialize();

    stbi_set_flip_vertically_on_load( true );
    df::cRenderer::initialize();
    df::cEventManager::initialize();
    df::cInputManager::initialize();
    df::cRenderCallbackManager::initialize();
    df::cQuadManager::initialize();
    df::cCameraManager::initialize();
}

cApplication::~cApplication()
{
#if defined ( DEBUG )
    PROFILING_BEGIN( __FUNCTION__ );
#endif

    df::cCameraManager::deinitialize();
    df::cQuadManager::deinitialize();
    df::cRenderCallbackManager::deinitialize();
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

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    df::cFreeFlightCamera flight_camera( 1, .1f );
    df::cCamera           camera2d( df::cCamera::kOrthographic, df::color::white, 90, 0 );

    df::cQuad quad( glm::vec3( 0, 0, 0 ), glm::vec2( .5f, .5f ), df::color::blue, "data/textures/wall.jpg" );

    df::cFont font( "data/fonts/MontserratMedium.ttf" );

    df::cRenderer::resizeWindow();
    df::cRenderer::setCursorInputMode( GLFW_CURSOR_DISABLED );

    while( !glfwWindowShouldClose( df::cRenderer::getWindow() ) )
    {
#if defined ( DEBUG )
        PROFILING_SCOPE( __FUNCTION__"::loop" );
#endif

        df::cInputManager::update();
        // df::cEventManager::invoke( df::event::update, m_timer.getDeltaSecond() );

        float delta_time = static_cast< float >( m_timer.getDeltaSecond() );
        flight_camera.update( delta_time );
        camera2d.update();

        flight_camera.beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        quad.render();
        flight_camera.endRender();

        camera2d.beginRender( GL_DEPTH_BUFFER_BIT );
        font.render( "Testing", glm::vec3( 50, 50, 0 ) );
        camera2d.endRender();

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
