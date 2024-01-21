#include "iRenderAsset.h"

#include <glad/glad.h>

#if PROFILING
#include "core/profiling/Profiling.h"
#endif

namespace df
{
    iRenderAsset::iRenderAsset( std::string _name )
    : iAsset( std::move( _name ) ),
      render_callback( nullptr )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glGenVertexArrays( 1, &vertex_array );
        glGenBuffers( 1, &m_vertex_buffer );
        glGenBuffers( 1, &m_element_buffer );
    }

    iRenderAsset::~iRenderAsset()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glDeleteBuffers( 1, &m_element_buffer );
        glDeleteBuffers( 1, &m_vertex_buffer );
        glDeleteVertexArrays( 1, &vertex_array );
    }
}
