#pragma once

#include "cPipeline.h"
#include "engine/managers/assets/iAssetManager.h"

namespace df::vulkan
{
    class cPipelineManager final : public iAssetManager< cPipelineManager, cPipeline >
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cPipelineManager )

        cPipelineManager()           = default;
        ~cPipelineManager() override = default;

        cPipeline* current;
    };
}
