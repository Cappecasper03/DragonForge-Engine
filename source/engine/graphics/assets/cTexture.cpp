#include "cTexture.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/assets/cTexture_opengl.h"
#include "engine/graphics/vulkan/assets/cTexture_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"
#include "iTexture.h"

df::cTexture::cTexture( const std::string& _name )
	: m_data( nullptr )
{
	switch( cRenderer::getDeviceType() )
	{
		case cRenderer::kOpenGl: m_data = new opengl::cTexture_opengl( _name ); break;
		case cRenderer::kVulkan: m_data = new vulkan::cTexture_vulkan( _name ); break;
	}
}

df::cTexture::~cTexture()
{
	DF_ProfilingScopeCpu;

	delete m_data;
}