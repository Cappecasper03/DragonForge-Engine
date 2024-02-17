#include "cFontManager.h"

#include "../cRenderCallbackManager.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/OpenGL/callbacks/DefaultFontCB.h"

namespace df
{
    cFontManager::cFontManager()
    {
        ZoneScoped;

        switch( cRendererSingleton::getRenderInstanceType() )
        {
            case cRendererSingleton::kOpenGL: { m_default_render_callback = cRenderCallbackManager::create( "default_font", opengl::render_callback::defaultFont ); }
            break;
            case cRendererSingleton::kVulkan:
            {}
            break;
        }
    }
}
