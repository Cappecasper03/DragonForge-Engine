#include "cRendererSingleton.h"

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "engine/filesystem/cFileSystem.h"
#include "OpenGL/cRenderer.h"
#include "Vulkan/cRenderer.h"

namespace df
{
	cRendererSingleton::cRendererSingleton( const eInstanceType _type )
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
				m_instance = new opengl::cRenderer();
				glfwSetWindowIcon( static_cast< GLFWwindow* >( m_instance->getWindow() ), 1, &icon );
			}
			break;
			case kVulkan:
			{
				m_instance = new vulkan::cRenderer();
				glfwSetWindowIcon( static_cast< GLFWwindow* >( m_instance->getWindow() ), 1, &icon );
			}
			break;
		}
	}

	cRendererSingleton::~cRendererSingleton()
	{
		ZoneScoped;

		delete m_instance;
	}
}
