#pragma once

#include "cApplication.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/rendering/assets/cameras/cFreeFlightCamera.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/iRenderer.h"
#include "engine/rendering/OpenGL/assets/cQuad_opengl.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"

class cTesting
{
public:
	cTesting();
	~cTesting();

	void render();
	void input( const df::input::sInput& _input );

	df::cFreeFlightCamera* camera;
};

inline cTesting::cTesting()
{
	/*auto quad = */df::cQuadManager::load( "quad", glm::vec3( 0, 0, 0 ), glm::vec2( 6, 4 ), df::color::blue );
	// quad->loadTexture( "data/resources/window.png", true );
	// df::cModelManager::load( "backpack", "data/models/survival-guitar-backpack" );

	camera = new df::cFreeFlightCamera( "freeflight", 1, .1f );
	camera->setActive( true );

	df::cEventManager::subscribe( df::event::update, camera, &df::cFreeFlightCamera::update );
	df::cEventManager::subscribe( df::event::render_3d, this, &cTesting::render );
	df::cEventManager::subscribe( df::event::input, this, &cTesting::input );

	df::cRenderer::getRenderInstance()->setCursorInputMode( GLFW_CURSOR_DISABLED );
}

inline cTesting::~cTesting()
{
	df::cEventManager::unsubscribe( df::event::input, this );
	df::cEventManager::unsubscribe( df::event::render_3d, this );
	df::cEventManager::unsubscribe( df::event::update, camera );
}

inline void cTesting::render()
{
	camera->beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	// df::cQuadManager::render();
	// df::cModelManager::render();
	camera->endRender();
}

inline void cTesting::input( const df::input::sInput& /*_input*/ )
{
	if( df::cInputManager::checkKey( GLFW_KEY_ESCAPE ) == df::input::kPress )
		cApplication::quit();
}
