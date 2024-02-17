#include "cQuadManager.h"

#include "../cRenderCallbackManager.h"
#include "engine/rendering/OpenGL/callbacks/DefaultQuadCB.h"

namespace df
{
    cQuadManager::cQuadManager()
    {
        ZoneScoped;

        switch( cRendererSingleton::getRenderInstanceType() )
        {
            case cRendererSingleton::kOpenGL: { m_default_render_callback = cRenderCallbackManager::create( "default_quad", opengl::render_callback::defaultQuad ); }
            break;
            case cRendererSingleton::kVulkan:
            {}
            break;
        }
    }
}
