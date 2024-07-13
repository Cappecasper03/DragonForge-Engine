﻿#include "cRenderer.h"

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "engine/filesystem/cFileSystem.h"
#include "OpenGL/cRenderer_opengl.h"
#include "vulkan/cDeferredRenderer_vulkan.h"
#include "Vulkan/cRenderer_vulkan.h"

namespace df
{
	cRenderer::cRenderer( const eInstanceType _type )
		: m_instance( nullptr )
		, m_type( _type )
		, m_is_deferred( true )
	{
		ZoneScoped;

		switch( _type )
		{
			case eOpenGL:
			{
				m_instance = new opengl::cRenderer_opengl();
			}
			break;
			case eVulkan:
			{
				m_instance = m_is_deferred ? new vulkan::cDeferredRenderer_vulkan() : new vulkan::cRenderer_vulkan();
			}
			break;
		}

		m_instance->initializeImGui();

		if( m_is_deferred )
			m_instance->initializeDeferred();

		int       channels;
		GLFWimage icon;
		icon.pixels = stbi_load( filesystem::getPath( "window.png" ).data(), &icon.width, &icon.height, &channels, 4 );
		glfwSetWindowIcon( m_instance->getWindow(), 1, &icon );
	}

	cRenderer::~cRenderer()
	{
		ZoneScoped;

		delete m_instance;
	}
}
