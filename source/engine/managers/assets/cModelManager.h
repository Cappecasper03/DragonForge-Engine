#pragma once

#include "iAssetManager.h"
#include "engine/rendering/assets/iModel.h"

namespace df
{
    class cModelManager final : public iAssetManager< cModelManager, iModel >
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cModelManager )

        cModelManager();
        ~cModelManager() override = default;
    };
}
