#include "cRenderTexture2D.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/assets/textures/cRenderTexture2D_opengl.h"
#include "engine/graphics/vulkan/assets/textures/cRenderTexture2D_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cRenderTexture2D* cRenderTexture2D::create( const sDescription& _description )
	{
		DF_ProfilingScopeCpu;

		cRenderTexture2D* texture = nullptr;

		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl:
			{
				texture = new opengl::cRenderTexture2D_opengl();
				break;
			}
			case cRenderer::kVulkan:
			{
				texture = new vulkan::cRenderTexture2D_vulkan();
				break;
			}
		}

		if( !texture )
			return nullptr;

		texture->initialize( _description );
		texture->clear( color::white );

		return texture;
	}
}