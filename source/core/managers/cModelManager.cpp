#include "cModelManager.h"

#include "cRenderCallbackManager.h"
#include "core/rendering/callbacks/DefaultMeshCB.h"

namespace df
{
    cModelManager::cModelManager()
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        const std::vector< std::string > shader_names = { "default_mesh_ambient" };
        m_default_render_callback                     = cRenderCallbackManager::create( "default_mesh", shader_names, render_callback::defaultMesh );
    }
}
