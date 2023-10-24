#include "cQuadManager.h"

#include "cRenderCallbackManager.h"
#include "core/rendering/callbacks/DefaultQuadCB.h"

namespace df
{
    cQuadManager::cQuadManager()
    {
        m_default_render_callback = cRenderCallbackManager::create( "default_quad", render_callback::defaultQuad );
    }
}
