#include "cFontManager.h"

#include "cRenderCallbackManager.h"
#include "core/rendering/callbacks/DefaultFontCB.h"

namespace df
{
    cFontManager::cFontManager()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        m_default_render_callback = cRenderCallbackManager::create( "default_font", render_callback::defaultFont );
    }
}
