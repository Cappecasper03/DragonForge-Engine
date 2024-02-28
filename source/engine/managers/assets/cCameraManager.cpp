#include "cCameraManager.h"

#include "engine/rendering/assets/cameras/cCamera.h"

namespace df
{
    cCameraManager::cCameraManager()
    {
        ZoneScoped;

        current = create( "default_3d", cCamera::eType::kPerspective, cColor( .5f, .75f, 1, 1 ), 90.f );
        create( "default_2d", cCamera::eType::kOrthographic, color::white, 90.f, 0.f, 100.f );
    }
}
