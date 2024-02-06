#include "cLightManager.h"

namespace df
{
    cLightManager::cLightManager()
    {
        create( "ambient", cLight::eType::kAmbient, 1.f );
    }
}
