#pragma once

#include "iAssetManager.h"
#include "core/rendering/assets/cLight.h"

namespace df
{
    class cLightManager final : public iAssetManager< cLightManager, cLight >
    {
    public:
        DISABLE_COPY_AND_MOVE( cLightManager )

        cLightManager()           = default;
        ~cLightManager() override = default;
    };
}
