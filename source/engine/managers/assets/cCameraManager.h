#pragma once

#include "iAssetManager.h"
#include "engine/rendering/assets/cameras/cCamera.h"

namespace df
{
    class cCameraManager final : public iAssetManager< cCameraManager, cCamera >
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cCameraManager );

        cCameraManager();
        ~cCameraManager() override = default;

        cCamera* current;
    };
}
