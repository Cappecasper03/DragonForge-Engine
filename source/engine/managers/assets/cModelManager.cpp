#include "cModelManager.h"

#include "../cRenderCallbackManager.h"
#include "engine/rendering/callbacks/DefaultMeshCB.h"

namespace df
{
    cModelManager::cModelManager()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        m_default_render_callback = cRenderCallbackManager::create( "default_mesh_deferred", render_callback::defaultMeshDeferred );
    }
}
