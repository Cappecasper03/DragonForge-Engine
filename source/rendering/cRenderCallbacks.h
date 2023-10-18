#pragma once

#include "core/misc/iSingleton.h"

namespace df
{
    class cShader;

    class cRenderCallbacks final : public iSingleton< cRenderCallbacks >
    {
    public:
        DISABLE_COPY_AND_MOVE( cRenderCallbacks );

        cRenderCallbacks();
        ~cRenderCallbacks() override;

    private:
    };
}
