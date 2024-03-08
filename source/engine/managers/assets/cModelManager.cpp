#include "cModelManager.h"

#include "../cRenderCallbackManager.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/iRenderer.h"
#include "engine/rendering/opengl/assets/cModel.h"
#include "engine/rendering/OpenGL/callbacks/DefaultMeshCB.h"
#include "engine/rendering/vulkan/assets/cModel.h"

namespace df
{
	cModelManager::cModelManager()
	{
		ZoneScoped;

		switch( cRendererSingleton::getInstanceType() )
		{
			case cRendererSingleton::kOpenGL:
			{
				if( cRendererSingleton::getRenderInstance()->isDeferred() )
					m_default_render_callback = cRenderCallbackManager::create( "default_mesh_deferred", opengl::render_callback::defaultMeshDeferred );
				else
				{
					const std::vector< std::string > shader_names = { "default_mesh_ambient" };
					m_default_render_callback                     = cRenderCallbackManager::create( "default_mesh", shader_names, opengl::render_callback::defaultMesh );
				}
			}
			break;
			case cRendererSingleton::kVulkan:
			{
			}
			break;
		}
	}

	iModel* cModelManager::create( const std::string& _name )
	{
		switch( cRendererSingleton::getInstanceType() )
		{
			case cRendererSingleton::kOpenGL:
				return iAssetManager::create< opengl::cModel >( _name );
			case cRendererSingleton::kVulkan:
				return iAssetManager::create< vulkan::cModel >( _name );
		}

		return nullptr;
	}
}
