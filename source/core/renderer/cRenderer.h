#pragma once
#include "core/misc/iSingleton.h"

namespace vg
{
    class cRenderer final : public vg::iSingleton< cRenderer >
    {
    public:
        DISABLE_COPY_AND_MOVE( cRenderer );

        cRenderer();
        ~cRenderer() override;
    };
}
