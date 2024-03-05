#include "cCameraManager.h"

#include "engine/rendering/assets/cameras/cCamera.h"
#include "engine/rendering/assets/cameras/cFreeFlightCamera.h"

namespace df
{
    cCameraManager::cCameraManager()
    {
        ZoneScoped;

        create( "default_3d", cCamera::eType::kPerspective, cColor( .5f, .75f, 1, 1 ), 90.f );
        create( "default_2d", cCamera::eType::kOrthographic, color::white, 90.f, 0.f, 100.f );

        current = new cFreeFlightCamera( "freeflight", 1, .1f );
        add( current );
    }
}
