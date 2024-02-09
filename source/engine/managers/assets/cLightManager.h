#pragma once

#include "iAssetManager.h"
#include "engine/rendering/assets/cLight.h"

namespace df
{
    class cLightManager final : public iAssetManager< cLightManager, cLight >
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cLightManager )

        cLightManager();
        ~cLightManager() override = default;
    };
}
