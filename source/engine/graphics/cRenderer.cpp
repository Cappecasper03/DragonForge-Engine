#include "cRenderer.h"

#include <stb_image.h>

#include "engine/profiling/ProfilingMacros.h"
#include "opengl/cGraphicsApi_opengl.h"
#include "vulkan/cGraphicsApi_vulkan.h"
#include "window/iWindow.h"

namespace df
{
	cRenderer::cRenderer( const eGraphicsApi _type, const std::string& _window_name )
		: m_graphics_api( nullptr )
		, m_type( _type )
		, m_is_deferred( false )
	{
		DF_ProfilingScopeCpu;

		switch( _type )
		{
			case kOpenGl:
			{
				m_graphics_api = new opengl::cGraphicsApi_opengl( _window_name );
				break;
			}
			case kVulkan:
			{
				m_graphics_api = new vulkan::cGraphicsApi_vulkan( _window_name );
				break;
			}
		}

		m_graphics_api->initialize();
		m_graphics_api->getWindow()->loadIcon();
		m_graphics_api->initializeImGui();
		m_graphics_api->initializeGui();

		if( m_is_deferred )
			m_graphics_api->initializeDeferred();
	}

	cRenderer::~cRenderer()
	{
		DF_ProfilingScopeCpu;

		delete m_graphics_api;
	}
}
