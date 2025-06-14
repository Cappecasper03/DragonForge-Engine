#include "cRenderTextureCamera2D.h"

#include "engine/graphics/api/iGraphicsApi.h"
#include "engine/graphics/assets/textures/cRenderTexture2D.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/cameras/cRenderTextureCamera2D_opengl.h"
#include "engine/graphics/vulkan/cameras/cRenderTextureCamera2D_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cRenderTextureCamera2D::cRenderTextureCamera2D( const sDescription& _description )
		: cCamera( _description )
	{}

	void cRenderTextureCamera2D::createTexture( const cRenderTexture2D::sDescription& _description )
	{
		DF_ProfilingScopeCpu;

		m_textures.push_back( cRenderTexture2D::create( _description ) );
	}

	cUnique< cRenderTextureCamera2D > cRenderTextureCamera2D::create( const sDescription& _description )
	{
		DF_ProfilingScopeCpu;

		cUnique< cRenderTextureCamera2D > camera = nullptr;

		switch( cRenderer::getApiType() )
		{
			case cRenderer::kOpenGl:
			{
				camera = MakeUnique< opengl::cRenderTextureCamera2D_opengl >( _description );
				break;
			}
			case cRenderer::kVulkan:
			{
				camera = MakeUnique< vulkan::cRenderTextureCamera2D_vulkan >( _description );
				break;
			}
		}

		if( !camera )
			return nullptr;

		return camera;
	}
}
