#pragma once

#include "cApplication.h"
#include "engine/managers/cInputManager.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/rendering/assets/cameras/cFreeFlightCamera.h"
#include "engine/rendering/OpenGL/assets/cQuad.h"
#include "engine/rendering/opengl/callbacks/DefaultMeshCB.h"
#include "engine/rendering/Vulkan/cRenderer.h"

class cTesting
{
public:
    cTesting();
    ~cTesting();

    void render();
    void input( const df::input::sInput& _input );

    df::cFreeFlightCamera* camera = new df::cFreeFlightCamera( "freeflight", 1, .1f );
};

inline cTesting::cTesting()
{
    camera->setActive( true );
    df::cQuadManager::create< df::opengl::cQuad >( "quad", glm::vec3( 0, 0, 0 ), glm::vec2( 600, 400 ), df::color::cyan );
    // df::cModelManager::create( "backpack", "data/models/survival-guitar-backpack" );
    df::cCameraManager::getInstance()->current = camera;

    df::cEventManager::subscribe( df::event::update, camera, &df::cFreeFlightCamera::update );
    df::cEventManager::subscribe( df::event::render_3d, this, &cTesting::render );
    df::cEventManager::subscribe( df::event::input, this, &cTesting::input );
}

inline cTesting::~cTesting()
{
    df::cEventManager::unsubscribe( df::event::input, this );
    df::cEventManager::unsubscribe( df::event::render_3d, this );
    df::cEventManager::unsubscribe( df::event::update, camera );

    delete camera;
}

inline void cTesting::render()
{
    // camera->beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // df::cModelManager::render();
    // camera->endRender();
}

inline void cTesting::input( const df::input::sInput& /*_input*/ )
{
    if( df::cInputManager::checkKey( GLFW_KEY_ESCAPE ) == df::input::kPress )
        cApplication::quit();
}
