#include "iRenderAsset.h"

#include <glad/glad.h>

namespace df
{
    iRenderAsset::iRenderAsset()
    : matrix( 1 )
    {
        glGenVertexArrays( 1, &vertex_array_object );
        glGenBuffers( 1, &m_vbo );
        glGenBuffers( 1, &m_ebo );
    }

    iRenderAsset::~iRenderAsset()
    {
        glDeleteBuffers( 1, &m_ebo );
        glDeleteBuffers( 1, &m_vbo );
        glDeleteVertexArrays( 1, &vertex_array_object );
    }
}
