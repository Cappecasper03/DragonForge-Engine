﻿#include "cRenderer.h"

#include <stb_image.h>

#include "engine/filesystem/cFileSystem.h"
#include "engine/profiling/ProfilingMacros.h"
#include "opengl/cRenderer_opengl.h"
#include "vulkan/cDeferredRenderer_vulkan.h"
#include "vulkan/cRenderer_vulkan.h"

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
			case eOpenGL:
			{
				m_instance = new opengl::cRenderer_opengl( _window_name );
			}
			break;
			case eVulkan:
			{
				m_instance = m_is_deferred ? new vulkan::cDeferredRenderer_vulkan( _window_name ) : new vulkan::cRenderer_vulkan( _window_name );
			}
			break;
		}

		m_instance->initializeImGui();

		if( m_is_deferred )
			m_instance->initializeDeferred();

		int channels;

		SDL_Surface icon;
		icon.pixels = stbi_load( filesystem::getPath( "window.png" ).data(), &icon.w, &icon.h, &channels, 4 );
		SDL_SetWindowIcon( m_instance->getWindow(), &icon );
	}

	cRenderer::~cRenderer()
	{
		DF_ProfilingScopeCpu;

		delete m_instance;
	}
}
