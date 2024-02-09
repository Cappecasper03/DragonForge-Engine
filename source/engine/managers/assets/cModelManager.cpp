#include "cModelManager.h"

#include "../cRenderCallbackManager.h"
#include "engine/rendering/callbacks/DefaultMeshCB.h"

namespace df
{
    cModelManager::cModelManager()
    {
        ZoneScoped;

        if( cRenderer::isDeferred() )
            m_default_render_callback = cRenderCallbackManager::create( "default_mesh_deferred", render_callback::defaultMeshDeferred );
        else
        {
            const std::vector< std::string > shader_names = { "default_mesh_ambient" };
            m_default_render_callback                     = cRenderCallbackManager::create( "default_mesh", shader_names, render_callback::defaultMesh );
        }
    }
}
