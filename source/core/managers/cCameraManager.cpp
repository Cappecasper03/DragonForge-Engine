#include "cCameraManager.h"

#include "core/rendering/assets/cameras/cCamera.h"

namespace df
{
    cCameraManager::cCameraManager()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        current = create( "default3D", cCamera::eType::kPerspective, cColor( .5f, .75f, 1, 1 ), 90.f );
        create( "default2D", cCamera::eType::kOrthographic, cColor( .5f, .75f, 1, 1 ), 90.f, 0.f, 100.f );
    }
}
