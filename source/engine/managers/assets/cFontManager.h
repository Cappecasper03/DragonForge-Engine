#pragma once

#include "iAssetManager.h"
#include "engine/rendering/assets/cFont.h"

namespace df
{
    class cFontManager final : public iAssetManager< cFontManager, cFont >
    {
    public:
        DISABLE_COPY_AND_MOVE( cFontManager )

        cFontManager();
        ~cFontManager() override = default;
    };
}
