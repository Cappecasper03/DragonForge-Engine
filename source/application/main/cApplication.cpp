#include "cApplication.h"

#include <cstring>
#include <filesystem>
#include <freetype/freetype.h>
#include <limits>

#ifdef DF_Windows
	#include <windows.h>
#elifdef DF_Linux
	#include <unistd.h>
#endif

#include "application/editor/cEditor.h"
#include "engine/core/cFileSystem.h"
#include "engine/core/utils/cTimer.h"
#include "engine/graphics/api/iGraphicsApi.h"
#include "engine/graphics/cRenderer.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cFontManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/managers/cLightManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cApplication::cApplication()
		: m_running( false )
	{
		DF_ProfilingScopeCpu;

		initializeEngine();

		cEventManager::initialize();
		cRenderer::initialize( cRenderer::eGraphicsApi::kOpenGl, m_name );
		cRenderCallbackManager::initialize();
		cLightManager::initialize();
		cQuadManager::initialize();
		cModelManager::initialize();
		cCameraManager::initialize();
		cInputManager::initialize();
		cFontManager::initialize();
	}

	cApplication::~cApplication()
	{
		DF_ProfilingScopeCpu;

		cFontManager::deinitialize();
		cInputManager::deinitialize();
		cCameraManager::deinitialize();
		cModelManager::deinitialize();
		cQuadManager::deinitialize();
		cLightManager::deinitialize();
		cRenderCallbackManager::deinitialize();
		cRenderer::deinitialize();
		cEventManager::deinitialize();
	}

	void cApplication::run()
	{
		DF_ProfilingScopeCpu;

		cApplication* application = getInstance();
		if( application->m_running )
			return;

		cEditor::initialize();
		application->m_running = true;

		iGraphicsApi* graphics_api = cRenderer::getApi();
		graphics_api->resizeWindow();

		while( application->m_running )
		{
			DF_ProfilingScopeNamedCpu( "frame" );

			cInputManager::update();
			cEventManager::invoke( event::update, static_cast< float >( application->m_timer.getDeltaSecond() ) );
			graphics_api->render();
			DF_ProfilingFrameMark;
		}

		cEditor::deinitialize();
	}

	void cApplication::quit()
	{
		DF_ProfilingScopeCpu;

		getInstance()->m_running = false;
	}

	void cApplication::initializeEngine()
	{
		DF_ProfilingScopeCpu;

#if defined( DF_Debug ) && defined( DF_Windows )
		AllocConsole();

		FILE* stdout_file = nullptr;
		FILE* stderr_file = nullptr;
		freopen_s( &stdout_file, "CONOUT$", "w", stdout );
		freopen_s( &stderr_file, "CONOUT$", "w", stderr );

		const HANDLE console_handle = GetStdHandle( STD_OUTPUT_HANDLE );
		DWORD        console_mode;
		GetConsoleMode( console_handle, &console_mode );

		console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode( console_handle, console_mode );

		SetConsoleTitle( "DragonForge-Engine Logs" );
#endif

		std::filesystem::path executable_path;
#ifdef DF_Windows
		wchar_t wbuffer[ MAX_PATH ];
		LPSTR   buffer = reinterpret_cast< LPSTR >( &wbuffer );
		GetModuleFileName( nullptr, buffer, MAX_PATH );
		executable_path = std::filesystem::path( buffer );
#elifdef DF_Linux
		char    buffer[ PATH_MAX ];
		ssize_t count = readlink( "/proc/self/exe", buffer, PATH_MAX );
		if( count != -1 )
			executable_path = std::filesystem::path( std::string( buffer, count ) );
#endif

		cFileSystem::setGameDirectory( executable_path.parent_path().parent_path().parent_path().string() + "/" );
		m_name = executable_path.filename().replace_extension().string();

		cFileSystem::remove( "binaries/log.csv" );
		cFileSystem::write( "binaries/log.csv", "Type;;Function;;Line;;Message\n", std::ios::out | std::ios::app );

		DF_LogRaw( "Starting DragonForge-Engine" );
	}
}
