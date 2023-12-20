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
#include "core/rendering/assets/cFont.h"
#include "core/rendering/assets/cModel.h"
#include "core/rendering/assets/cQuad.h"
#include "core/rendering/assets/cameras/cFreeFlightCamera.h"

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
    df::cCamera           camera2d( "orthographic", df::cCamera::kOrthographic, df::color::white, 90, 0 );

    df::cQuad quad( "test", glm::vec3( 0, 0, 0 ), glm::vec2( .5f, .5f ), df::color::blue, "data/textures/wall.jpg" );

    df::cFont font( "test", "data/fonts/MontserratMedium.ttf" );

    df::cModel model( "test", "data/models/survival-guitar-backpack" );

    df::cRenderer::resizeWindow();
    df::cRenderer::setCursorInputMode( GLFW_CURSOR_DISABLED );

    float fps = 0;

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

        fps = glm::mix( fps, 1 / delta_time, delta_time );

        flight_camera.beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        quad.render();
        model.render();
        flight_camera.endRender();

        camera2d.beginRender( GL_DEPTH_BUFFER_BIT );
        font.render( std::format( "FPS: {:.0f}", fps ), glm::vec3( 10, 785, 0 ), glm::vec2( .3f ) );
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
