#include "iRenderAsset.h"

#include <glad/glad.h>

namespace df
{
    iRenderAsset::iRenderAsset( std::string _name )
    : iAsset( std::move( _name ) ),
      render_callback( nullptr )
    {
        glGenVertexArrays( 1, &vertex_array );
        glGenBuffers( 1, &m_vertex_buffer );
        glGenBuffers( 1, &m_element_buffer );
    }

    iRenderAsset::~iRenderAsset()
    {
        glDeleteBuffers( 1, &m_element_buffer );
        glDeleteBuffers( 1, &m_vertex_buffer );
        glDeleteVertexArrays( 1, &vertex_array );
    }
}
