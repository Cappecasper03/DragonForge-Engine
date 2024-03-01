#include "sRenderAsset.h"

#include <glad/glad.h>
#include <tracy/Tracy.hpp>

#include "engine/rendering/Vulkan/cRenderer.h"

namespace df::opengl
{
    sRenderAsset::sRenderAsset( std::string _name )
    : iRenderAsset( std::move( _name ) )
    {
        ZoneScoped;

        glGenVertexArrays( 1, &vertex_array );
        glGenBuffers( 1, &vertex_buffer );
        glGenBuffers( 1, &element_buffer );
    }

    sRenderAsset::~sRenderAsset()
    {
        ZoneScoped;

        glDeleteBuffers( 1, &element_buffer );
        glDeleteBuffers( 1, &vertex_buffer );
        glDeleteVertexArrays( 1, &vertex_array );
    }
}
