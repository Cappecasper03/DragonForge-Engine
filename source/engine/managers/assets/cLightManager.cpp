#include "cLightManager.h"

namespace df
{
    cLightManager::cLightManager()
    {
        ZoneScoped;

        create( "ambient", cLight::eType::kAmbient, 1.f );
    }
}
