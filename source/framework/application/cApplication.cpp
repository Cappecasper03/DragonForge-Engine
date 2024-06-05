#include "cApplication.h"

#include <filesystem>
#include <freetype/freetype.h>
#include <GLFW/glfw3.h>
#include <tracy/Tracy.hpp>
#include <windows.h>

#include "cTesting.h"
#include "engine/filesystem/cFileSystem.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/misc/cTimer.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/iRenderer.h"

cApplication::cApplication()
	: m_fps( 0 )
	, m_running( true )
{
	ZoneScoped;

	initializeEngine();

	df::cEventManager::initialize();
	df::cRenderer::initialize( df::cRenderer::eInstanceType::kVulkan );
	df::cRenderCallbackManager::initialize();
	df::cQuadManager::initialize();
	df::cModelManager::initialize();
	df::cCameraManager::initialize();
	df::cInputManager::initialize();
}

cApplication::~cApplication()
{
	ZoneScoped;

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
	ZoneScoped;

	cTesting* testing = new cTesting();

	cApplication*  application     = getInstance();
	df::iRenderer* render_instance = df::cRenderer::getRenderInstance();
	render_instance->resizeWindow();

	while( !glfwWindowShouldClose( render_instance->getWindow() ) )
	{
		const double delta_second = application->m_timer.getDeltaSecond();
		const double target_fps   = 1.f / delta_second;
		application->m_fps        = application->m_fps + ( target_fps - application->m_fps ) * .1f * delta_second;

		df::cInputManager::update();
		df::cEventManager::invoke( df::event::update, static_cast< float >( delta_second ) );
		render_instance->render();
		FrameMark;
	}

	delete testing;
}

void cApplication::quit()
{
	ZoneScoped;

	glfwSetWindowShouldClose( df::cRenderer::getRenderInstance()->getWindow(), true );
}

void cApplication::initializeEngine()
{
	ZoneScoped;

	SetConsoleTitle( L"DragonForge-Engine Logs" );

	size_t  size;
	wchar_t wbuffer[ MAX_PATH ];
	char    buffer[ MAX_PATH ];

	GetModuleFileName( nullptr, wbuffer, MAX_PATH );
	wcstombs_s( &size, buffer, MAX_PATH, wbuffer, MAX_PATH );
	const std::filesystem::path executable_path( buffer );

	df::filesystem::setGameDirectory( executable_path.parent_path().parent_path().string() + "\\" );
	m_name = executable_path.filename().replace_extension().string();

	df::filesystem::remove( "log.csv" );
	df::filesystem::write( "log.csv", "Type;;Function;;Line;;Message\n", std::ios::out | std::ios::app );

	DF_LOG_RAW( "Starting DragonForge-Engine" );
}
