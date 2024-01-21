#pragma once

#include "iAssetManager.h"
#include "core/rendering/assets/cModel.h"

namespace df
{
    class cModelManager final : public iAssetManager< cModelManager, cModel >
    {
    public:
        DISABLE_COPY_AND_MOVE( cModelManager )

        cModelManager();
        ~cModelManager() override = default;
    };
}
