#include "iRenderAsset.h"

#include <glad/glad.h>
#include <tracy/Tracy.hpp>

namespace df
{
    iRenderAsset::iRenderAsset( std::string _name )
    : iAsset( std::move( _name ) ),
      render_callback( nullptr )
    {
        ZoneScoped;
        
        glGenVertexArrays( 1, &vertex_array );
        glGenBuffers( 1, &m_vertex_buffer );
        glGenBuffers( 1, &m_element_buffer );
    }

    iRenderAsset::~iRenderAsset()
    {
        ZoneScoped;
        
        glDeleteBuffers( 1, &m_element_buffer );
        glDeleteBuffers( 1, &m_vertex_buffer );
        glDeleteVertexArrays( 1, &vertex_array );
    }
}
