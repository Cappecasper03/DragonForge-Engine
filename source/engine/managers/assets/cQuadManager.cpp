#include "cQuadManager.h"

#include "../cRenderCallbackManager.h"
#include "engine/rendering/callbacks/DefaultQuadCB.h"

namespace df
{
    cQuadManager::cQuadManager()
    {
        ZoneScoped;

        m_default_render_callback = cRenderCallbackManager::create( "default_quad", render_callback::defaultQuad );
    }
}
