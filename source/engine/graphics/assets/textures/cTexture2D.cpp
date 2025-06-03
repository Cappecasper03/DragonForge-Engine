#include "cTexture2D.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/assets/cTexture2D_opengl.h"
#include "engine/graphics/vulkan/assets/cTexture2D_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cTexture2D::cTexture2D( const std::string& _name )
		: m_name( _name )
	{}

	cTexture2D* cTexture2D::create( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		switch( cRenderer::getDeviceType() )
		{
			case cRenderer::kOpenGl: return new opengl::cTexture2D_opengl( _name );
			case cRenderer::kVulkan: return new vulkan::cTexture2D_vulkan( _name );
		}

		return nullptr;
	}
}
