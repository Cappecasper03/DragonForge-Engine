#pragma once

#include "iAssetManager.h"
#include "engine/rendering/assets/cModel.h"

namespace df
{
    class cModelManager final : public iAssetManager< cModelManager, cModel >
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cModelManager )

        cModelManager();
        ~cModelManager() override = default;
    };
}
