#include "cApplication.h"

#include <cstring>
#include <filesystem>
#include <freetype/freetype.h>
#include <limits>

#ifdef DF_Windows
	#include <windows.h>
#elif defined( DF_Linux )
	#include <unistd.h>
#endif

#include "cTesting.h"
#include "engine/core/cFileSystem.h"
#include "engine/graphics/cRenderer.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/core/utils/cTimer.h"
#include "engine/profiling/ProfilingMacros.h"

cApplication::cApplication()
	: m_fps( 0 )
	, m_running( false )
{
	DF_ProfilingScopeCpu;

	initializeEngine();

	df::cEventManager::initialize();
	df::cRenderer::initialize( df::cRenderer::eInstanceType::kVulkan, m_name );
	df::cRenderCallbackManager::initialize();
	df::cQuadManager::initialize();
	df::cModelManager::initialize();
	df::cCameraManager::initialize();
	df::cInputManager::initialize();
}

cApplication::~cApplication()
{
	DF_ProfilingScopeCpu;

	df::cInputManager::deinitialize();
	df::cCameraManager::deinitialize();
	df::cModelManager::deinitialize();
	df::cQuadManager::deinitialize();
	df::cRenderCallbackManager::deinitialize();
	df::cRenderer::deinitialize();
	df::cEventManager::deinitialize();
}

void cApplication::run()
{
	DF_ProfilingScopeCpu;

	cApplication* application = getInstance();
	if( application->m_running )
		return;

	application->m_running = true;

	cTesting* testing = new cTesting();

	df::iRenderer* render_instance = df::cRenderer::getRenderInstance();
	render_instance->resizeWindow();

	while( application->m_running )
	{
		const double delta_second  = application->m_timer.getDeltaSecond();
		const double target_fps    = 1.f / delta_second;
		application->m_fps        += ( target_fps - application->m_fps ) * delta_second * 10.f;

		df::cInputManager::update();
		df::cEventManager::invoke( df::event::update, static_cast< float >( delta_second ) );
		render_instance->render();
		DF_ProfilingFrameMark;
	}

	delete testing;
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
#elif defined( DF_Linux )
	char    buffer[ PATH_MAX ];
	ssize_t count = readlink( "/proc/self/exe", buffer, PATH_MAX );
	if( count != -1 )
		executable_path = std::filesystem::path( std::string( buffer, count ) );
#endif

	df::cFileSystem::setGameDirectory( executable_path.parent_path().parent_path().parent_path().string() + "/" );
	m_name = executable_path.filename().replace_extension().string();

	df::cFileSystem::remove( "binaries/log.csv" );
	df::cFileSystem::write( "binaries/log.csv", "Type;;Function;;Line;;Message\n", std::ios::out | std::ios::app );

	DF_LogRaw( "Starting DragonForge-Engine" );
}
