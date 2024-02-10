#include "cRenderer.h"

#include <stb_image.h>
#include <GLFW/glfw3.h>

#include "engine/filesystem/cFileSystem.h"
#include "renderers/cOpenGLRenderer.h"
#include "renderers/cVulkanRenderer.h"

namespace df
{
    cRenderer::cRenderer( const eInstanceType _type )
    : m_instance( nullptr ),
      m_type( _type )
    {
        ZoneScoped;

        int       channels;
        GLFWimage icon;
        icon.pixels = stbi_load( filesystem::getPath( "window.png" ).data(), &icon.width, &icon.height, &channels, 4 );

        switch( _type )
        {
            case kOpenGL:
            {
                m_instance = new cOpenGLRenderer();
                glfwSetWindowIcon( reinterpret_cast< cOpenGLRenderer* >( m_instance )->m_window, 1, &icon );
            }
            break;
            case kVulkan: { m_instance = new cVulkanRenderer(); }
            break;
        }
    }

    cRenderer::~cRenderer()
    {
        delete m_instance;
    }
}
