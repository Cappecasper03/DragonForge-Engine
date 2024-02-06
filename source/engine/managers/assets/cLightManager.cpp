#include "cLightManager.h"

namespace df
{
    cLightManager::cLightManager()
    {
#ifdef PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        create( "ambient", cLight::eType::kAmbient, 1.f );
    }
}
