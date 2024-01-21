#pragma once

#include "iAssetManager.h"
#include "core/rendering/assets/cameras/cCamera.h"

namespace df
{
    class cCameraManager final : public iAssetManager< cCameraManager, cCamera >
    {
    public:
        DISABLE_COPY_AND_MOVE( cCameraManager );

        cCameraManager();
        ~cCameraManager() override = default;

        static bool add( cCamera* _camera );

        cCamera* current;
    };
}
