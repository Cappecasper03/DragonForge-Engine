#pragma once

#include "core/managers/assets/cCameraManager.h"
#include "core/managers/assets/cModelManager.h"
#include "core/rendering/assets/cameras/cFreeFlightCamera.h"

class cTesting
{
public:
    cTesting();
    ~cTesting();

    void render();

    df::cFreeFlightCamera* camera = new df::cFreeFlightCamera( "freeflight", 1, .1f );
};

inline cTesting::cTesting()
{
    camera->setActive( true );
    df::cModelManager::create( "backpack", "data/models/survival-guitar-backpack" );
    df::cCameraManager::getInstance()->current = camera;

    df::cEventManager::subscribe( df::event::update, camera, &df::cFreeFlightCamera::update );
    df::cEventManager::subscribe( df::event::render_3d, this, &cTesting::render );
}

inline cTesting::~cTesting()
{
    df::cEventManager::unsubscribe( df::event::render_3d, this );
    df::cEventManager::unsubscribe( df::event::update, camera );
}

inline void cTesting::render()
{
    camera->beginRender( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    df::cModelManager::render();
    camera->endRender();
}
