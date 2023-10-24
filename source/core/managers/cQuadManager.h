#pragma once

#include "iRenderAssetManager.h"

namespace df
{
    class cQuad;

    class cQuadManager final : public iRenderAssetManager< cQuadManager, cQuad >
    {
    public:
        DISABLE_COPY_AND_MOVE( cQuadManager )

        cQuadManager();
        ~cQuadManager() override = default;
    };
}
