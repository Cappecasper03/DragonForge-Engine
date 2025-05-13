#include "cRenderer.h"

#include <stb_image.h>

#include "engine/profiling/ProfilingMacros.h"
#include "opengl/cRenderer_opengl.h"
#include "vulkan/cRenderer_vulkan.h"
#include "window/iWindow.h"

namespace df
{
	cRenderer::cRenderer( const eInstanceType _type, const std::string& _window_name )
		: m_instance( nullptr )
		, m_type( _type )
		, m_is_deferred( false )
	{
		DF_ProfilingScopeCpu;

		switch( _type )
		{
			case kOpenGl:
			{
				m_instance = new opengl::cRenderer_opengl( _window_name );
				break;
			}
			case kVulkan:
			{
				m_instance = new vulkan::cRenderer_vulkan( _window_name );
				break;
			}
		}

		m_instance->getWindow()->loadIcon();
		m_instance->initializeImGui();

		if( m_is_deferred )
			m_instance->initializeDeferred();
	}

	cRenderer::~cRenderer()
	{
		DF_ProfilingScopeCpu;

		delete m_instance;
	}
}
