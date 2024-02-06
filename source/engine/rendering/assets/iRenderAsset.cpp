#include "iRenderAsset.h"

#include <glad/glad.h>

#ifdef PROFILING
#include "engine/profiling/Profiling.h"
#endif

namespace df
{
    iRenderAsset::iRenderAsset( std::string _name )
    : iAsset( std::move( _name ) ),
      render_callback( nullptr )
    {
#ifdef PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glGenVertexArrays( 1, &vertex_array );
        glGenBuffers( 1, &m_vertex_buffer );
        glGenBuffers( 1, &m_element_buffer );
    }

    iRenderAsset::~iRenderAsset()
    {
#ifdef PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glDeleteBuffers( 1, &m_element_buffer );
        glDeleteBuffers( 1, &m_vertex_buffer );
        glDeleteVertexArrays( 1, &vertex_array );
    }
}
