#include "cApplication.h"

#include <filesystem>
#include <windows.h>
#include <freetype/freetype.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "cTesting.h"
#include "engine/filesystem/cFileSystem.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/managers/assets/cFontManager.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/misc/cTimer.h"
#include "engine/rendering/cRenderer.h"

cApplication::cApplication()
: m_running( true )
{
    initialize();

    df::cRenderCallbackManager::initialize();
    df::cEventManager::initialize();
    df::cRenderer::initialize();
    df::cInputManager::initialize();
    df::cQuadManager::initialize();
    df::cFontManager::initialize();
    df::cModelManager::initialize();
    df::cCameraManager::initialize();
}

cApplication::~cApplication()
{
    df::cCameraManager::deinitialize();
    df::cModelManager::deinitialize();
    df::cFontManager::deinitialize();
    df::cQuadManager::deinitialize();
    df::cInputManager::deinitialize();
    df::cRenderer::deinitialize();
    df::cEventManager::deinitialize();
    df::cRenderCallbackManager::deinitialize();
}

void cApplication::run()
{
    cTesting* testing = new cTesting();
    testing;

    cApplication* application = getInstance();
    df::cRenderer::resizeWindow();
    df::cRenderer::setCursorInputMode( GLFW_CURSOR_DISABLED );

    while( application->m_running )
    {
        df::cInputManager::update();
        df::cEventManager::invoke( df::event::update, static_cast< float >( application->m_timer.getDeltaSecond() ) );
        df::cRenderer::render();
    }
}

void cApplication::initialize()
{
#ifdef DEBUG
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

    LOG_RAW( "Starting DragonForge-Engine" );
}
