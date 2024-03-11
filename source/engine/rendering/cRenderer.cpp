#include "cRenderer.h"

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "engine/filesystem/cFileSystem.h"
#include "OpenGL/cRenderer_opengl.h"
#include "Vulkan/cRenderer_vulkan.h"

namespace df
{
	cRenderer::cRenderer( const eInstanceType _type )
		: m_instance( nullptr )
		, m_type( _type )
	{
		ZoneScoped;

		int       channels;
		GLFWimage icon;
		icon.pixels = stbi_load( filesystem::getPath( "window.png" ).data(), &icon.width, &icon.height, &channels, 4 );

		switch( _type )
		{
			case kOpenGL:
			{
				m_instance = new opengl::cRenderer_opengl();
			}
			break;
			case kVulkan:
			{
				m_instance = new vulkan::cRenderer_vulkan();
			}
			break;
		}

		glfwSetWindowIcon( m_instance->getWindow(), 1, &icon );
	}

	cRenderer::~cRenderer()
	{
		ZoneScoped;

		delete m_instance;
	}
}
