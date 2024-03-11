#include "cModelManager.h"

#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/iRenderer.h"
#include "engine/rendering/opengl/assets/cModel_opengl.h"
#include "engine/rendering/opengl/callbacks/DefaultMeshCB_opengl.h"
#include "engine/rendering/vulkan/assets/cModel_vulkan.h"

namespace df
{
	cModelManager::cModelManager()
	{
		ZoneScoped;

		switch( cRenderer::getInstanceType() )
		{
			case cRenderer::kOpenGL:
			{
				if( cRenderer::getRenderInstance()->isDeferred() )
					m_default_render_callback = cRenderCallbackManager::create( "default_mesh_deferred", opengl::render_callback::defaultMeshDeferred );
				else
				{
					const std::vector< std::string > shader_names = { "default_mesh_ambient" };
					m_default_render_callback                     = cRenderCallbackManager::create( "default_mesh", shader_names, opengl::render_callback::defaultMesh );
				}
			}
			break;
			case cRenderer::kVulkan:
			{
			}
			break;
		}
	}

	iModel* cModelManager::load( const std::string& _name, const std::string& _folder_path, const unsigned _load_flags )
	{
		switch( cRenderer::getInstanceType() )
		{
			case cRenderer::kOpenGL:
			{
				iModel* model = create< opengl::cModel_opengl >( _name );
				model->load( _folder_path, _load_flags );
				return model;
			}
			case cRenderer::kVulkan:
			{
				iModel* model = create< vulkan::cModel_vulkan >( _name );
				model->load( _folder_path, _load_flags );
				return model;
			}
		}

		return nullptr;
	}
}
