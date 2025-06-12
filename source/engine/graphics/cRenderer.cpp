#include "cRenderer.h"

#include <stb_image.h>

#include "engine/profiling/ProfilingMacros.h"
#include "opengl/cGraphicsDevice_opengl.h"
#include "vulkan/cGraphicsDevice_vulkan.h"
#include "window/iWindow.h"

namespace df
{
	cRenderer::cRenderer( const eDeviceType _type, const std::string& _window_name )
		: m_graphics_device( nullptr )
		, m_type( _type )
		, m_is_deferred( false )
	{
		DF_ProfilingScopeCpu;

		switch( _type )
		{
			case kOpenGl:
			{
				m_graphics_device = new opengl::cGraphicsDevice_opengl( _window_name );
				break;
			}
			case kVulkan:
			{
				m_graphics_device = new vulkan::cGraphicsDevice_vulkan( _window_name );
				break;
			}
		}

		m_graphics_device->initialize();
		m_graphics_device->getWindow()->loadIcon();
		m_graphics_device->initializeImGui();
		m_graphics_device->initializeGui();

		if( m_is_deferred )
			m_graphics_device->initializeDeferred();
	}

	cRenderer::~cRenderer()
	{
		DF_ProfilingScopeCpu;

		delete m_graphics_device;
	}
}
