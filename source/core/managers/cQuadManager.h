#pragma once

#include "iAssetManager.h"
#include "core/rendering/assets/cQuad.h"

namespace df
{
    class cQuadManager final : public iAssetManager< cQuadManager, cQuad >
    {
    public:
        DISABLE_COPY_AND_MOVE( cQuadManager )

        cQuadManager();
        ~cQuadManager() override = default;
    };
}
