#pragma once

#include "iAssetManager.h"
#include "engine/rendering/assets/iFont.h"

namespace df
{
    class cFontManager final : public iAssetManager< cFontManager, cFont >
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cFontManager )

        cFontManager();
        ~cFontManager() override = default;
    };
}
