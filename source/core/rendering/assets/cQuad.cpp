#include "cQuad.h"

#include <glad/glad.h>

#include "core/managers/cRenderCallbackManager.h"
#include "core/memory/Memory.h"
#include "core/rendering/cShader.h"
#include "core/rendering/assets/cTexture.h"

namespace df
{
    cQuad::cQuad( const float& _width, const float& _height, const cColor& _color, const std::string& _texture_file )
    : color( _color ),
      texture( nullptr ),
      m_vertices{},
      m_indices{ 0, 1, 3, 1, 2, 3 }
    {
        m_vertices[ 0 ] = { glm::vec3( _width / 2, _height / 2, 0 ), glm::vec2( 1, 1 ) };
        m_vertices[ 1 ] = { glm::vec3( _width / 2, -_height / 2, 0 ), glm::vec2( 1, 0 ) };
        m_vertices[ 2 ] = { glm::vec3( -_width / 2, -_height / 2, 0 ), glm::vec2( 0, 0 ) };
        m_vertices[ 3 ] = { glm::vec3( -_width / 2, _height / 2, 0 ), glm::vec2( 0, 1 ) };

        glBindVertexArray( vertex_array_object );

        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof( m_vertices ), m_vertices, GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ebo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( m_indices ), m_indices, GL_STATIC_DRAW );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( *m_vertices ), nullptr );
        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( *m_vertices ), reinterpret_cast< void* >( sizeof( m_vertices->position ) ) );
        glEnableVertexAttribArray( 1 );

        if( !_texture_file.empty() )
        {
            texture = MEMORY_ALLOC( cTexture, 1, GL_TEXTURE_2D );
            texture->load( _texture_file );
        }

        render_callback = cRenderCallbackManager::get( "default_quad" );
    }

    cQuad::~cQuad()
    {
        if( texture )
            MEMORY_FREE( texture );
    }

    void cQuad::render()
    {
        cRenderCallbackManager::render( render_callback, this );
    }

    void cQuad::bindTexture( const int& _index ) const
    {
        if( texture )
            texture->bind( _index );
    }
}
