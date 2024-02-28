#include "iRenderAsset.h"

#include <glad/glad.h>
#include <tracy/Tracy.hpp>

#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/Vulkan/cRenderer.h"

namespace df
{
    iRenderAsset::iRenderAsset( std::string _name )
    : iAsset( std::move( _name ) ),
      render_callback( nullptr )
    {
        ZoneScoped;

        transform = new cTransform();

        switch( cRendererSingleton::getRenderInstanceType() )
        {
            case cRendererSingleton::kOpenGL: { render_specific = new opengl::sRendererSpecific{}; }
            break;
            case cRendererSingleton::kVulkan: { render_specific = new vulkan::sRendererSpecific{}; }
            break;
        }
    }

    namespace opengl
    {
        sRendererSpecific::sRendererSpecific()
        {
            ZoneScoped;

            glGenVertexArrays( 1, &vertex_array );
            glGenBuffers( 1, &vertex_buffer );
            glGenBuffers( 1, &element_buffer );
        }

        sRendererSpecific::~sRendererSpecific()
        {
            ZoneScoped;

            glDeleteBuffers( 1, &element_buffer );
            glDeleteBuffers( 1, &vertex_buffer );
            glDeleteVertexArrays( 1, &vertex_array );
        }
    }

    namespace vulkan
    {
        sRendererSpecific::sRendererSpecific()
        : vertex_buffer( nullptr ),
          vertex_buffer_memory( nullptr )
        {
            ZoneScoped;
        }

        sRendererSpecific::~sRendererSpecific()
        {
            ZoneScoped;

            const cRenderer* renderer = reinterpret_cast< cRenderer* >( cRendererSingleton::getRenderInstance() );

            vkDestroyBuffer( renderer->logical_device, vertex_buffer, nullptr );
            vkFreeMemory( renderer->logical_device, vertex_buffer_memory, nullptr );
        }
    }

    iRenderAsset::~iRenderAsset()
    {
        ZoneScoped;

        delete render_specific;
        delete transform;
    }
}
