#pragma once

#include "cApplication.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/math/cVector.h"
#include "engine/rendering/assets/cameras/cFreeFlightCamera.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/iRenderer.h"
#include "engine/rendering/opengl/assets/cQuad_opengl.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"
#include "engine/rendering/vulkan/descriptor/sDescriptorWriter_vulkan.h"
#include "engine/rendering/vulkan/pipeline/cPipeline_vulkan.h"
#include "imgui.h"

class cTesting
{
public:
	cTesting();
	~cTesting();

	void update( float _delta_time );
	void render3d();
	void render2d();
	void imgui();
	void input( const df::input::sInput& _input );

	df::cFreeFlightCamera*        camera;
	df::vulkan::cPipeline_vulkan* pipeline;
};

inline cTesting::cTesting()
{
	auto quad = df::cQuadManager::load( "quad", df::cVector3f( 300, 200, 0 ), df::cVector2f( 600, 400 ), df::color::blue );
	quad->loadTexture( "data/resources/window.png" );
	df::cModelManager::load( "model", "data/models/sponza" );

	camera = new df::cFreeFlightCamera( "freeflight", 1, .1f );
	camera->setActive( true );

	df::cEventManager::subscribe( df::event::update, camera, &df::cFreeFlightCamera::update );
	df::cEventManager::subscribe( df::event::render_3d, this, &cTesting::render3d );
	df::cEventManager::subscribe( df::event::render_2d, this, &cTesting::render2d );
	df::cEventManager::subscribe( df::event::imgui, this, &cTesting::imgui );
	df::cEventManager::subscribe( df::event::input, this, &cTesting::input );

	SDL_SetWindowRelativeMouseMode( df::cRenderer::getRenderInstance()->getWindow(), true );
	SDL_CaptureMouse( true );
}

inline cTesting::~cTesting()
{
	df::cEventManager::unsubscribe( df::event::input, this );
	df::cEventManager::unsubscribe( df::event::imgui, this );
	df::cEventManager::unsubscribe( df::event::render_2d, this );
	df::cEventManager::unsubscribe( df::event::render_3d, this );
	df::cEventManager::unsubscribe( df::event::update, camera );
}

inline void cTesting::render3d()
{
	camera->beginRender( df::cCamera::eColor | df::cCamera::eDepth );

	df::cModelManager::render();

	camera->endRender();
}

inline void cTesting::render2d()
{
	df::cCamera* camera2 = df::cCameraManager::get( "default_2d" );
	camera2->beginRender( df::cCamera::eDepth );

	df::cQuadManager::render();

	camera2->endRender();
}

inline void cTesting::imgui()
{
	if( ImGui::Begin( "Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		ImGui::Text( "FPS: %i", cApplication::getFps() );
		ImGui::End();
	}
}

inline void cTesting::input( const df::input::sInput& /*_input*/ )
{
	if( df::cInputManager::checkKey( df::input::eKey::kEscape ) == df::input::kPress )
		cApplication::quit();
}
