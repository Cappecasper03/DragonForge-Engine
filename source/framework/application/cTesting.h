#pragma once

#include "cApplication.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/rendering/assets/cameras/cFreeFlightCamera.h"
#include "engine/rendering/OpenGL/assets/cQuad.h"
#include "engine/rendering/vulkan/cRenderer.h"

class cTesting
{
public:
	cTesting();
	~cTesting();

	void render();
	void input( const df::input::sInput& _input );
};

inline cTesting::cTesting()
{
	df::cQuadManager::create( "quad", glm::vec3( 0, 0, 0 ), glm::vec2( 600, 400 ), df::color::cyan );
	// df::cModelManager::create( "backpack", "data/models/survival-guitar-backpack" );

	df::cEventManager::subscribe( df::event::update, reinterpret_cast< df::cFreeFlightCamera* >( df::cCameraManager::getInstance()->current ), &df::cFreeFlightCamera::update );
	df::cEventManager::subscribe( df::event::render_3d, this, &cTesting::render );
	df::cEventManager::subscribe( df::event::input, this, &cTesting::input );
}

inline cTesting::~cTesting()
{
	df::cEventManager::unsubscribe( df::event::input, this );
	df::cEventManager::unsubscribe( df::event::render_3d, this );
	df::cEventManager::unsubscribe( df::event::update, reinterpret_cast< df::cFreeFlightCamera* >( df::cCameraManager::getInstance()->current ) );
}

inline void cTesting::render()
{
	df::cCameraManager::getInstance()->current->beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	df::cModelManager::render();
	df::cCameraManager::getInstance()->current->endRender();
}

inline void cTesting::input( const df::input::sInput& /*_input*/ )
{
	if( df::cInputManager::checkKey( GLFW_KEY_ESCAPE ) == df::input::kPress )
		cApplication::quit();
}
