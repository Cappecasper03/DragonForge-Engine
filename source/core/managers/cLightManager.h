#pragma once

#include "iAssetManager.h"

namespace df
{
    struct sLight
    {};

    class cLightManager final : public iAssetManager< cLightManager, sLight >
    {
    public:
        DISABLE_COPY_AND_MOVE( cLightManager )

        cLightManager()           = default;
        ~cLightManager() override = default;
    };
}
