#include "cFont.h"

#include <format>
#include <freetype/freetype.h>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

#include "core/memory/Memory.h"

namespace df
{
    cFont::cFont( const std::string& _font_file )
    : m_bitmap( nullptr ),
      m_vertices{},
      m_indices{ 0, 1, 3, 1, 2, 3 }
    {
        if( !_font_file.empty() )
            load( _font_file );
    }

    cFont::~cFont()
    {
        if( m_bitmap )
            MEMORY_FREE( m_bitmap );
    }

    bool cFont::load( const std::string& _font_file )
    {
        m_characters.clear();

        FT_Library library;
        if( FT_Init_FreeType( &library ) )
        {
            LOG_ERROR( "Failed to initialize FreeType library" );
            return false;
        }

        FT_Face face;
        if( FT_New_Face( library, _font_file.c_str(), 0, &face ) )
        {
            LOG_ERROR( std::format( "Failed to load font: {}", _font_file ) );
            FT_Done_FreeType( library );
            return false;
        }

        constexpr int pixel_height = 48;
        FT_Set_Pixel_Sizes( face, 0, pixel_height );

        if( m_bitmap )
            MEMORY_FREE( m_bitmap );
        m_bitmap = MEMORY_ALLOC( cTexture, 1, GL_TEXTURE_2D_ARRAY );

        m_bitmap->bind();
        m_bitmap->setPixelStoreI( GL_UNPACK_ALIGNMENT, 1 );
        m_bitmap->setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        m_bitmap->setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        m_bitmap->setTextureParameterI( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        m_bitmap->setTextureParameterI( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        int width_max = 0;
        for( unsigned char c = 0; c < 128; ++c )
        {
            if( FT_Load_Char( face, c, FT_LOAD_RENDER ) )
            {
                LOG_ERROR( std::format( "Failed to load glyph: {}", c ) );
                continue;
            }

            sCharacter character = {};
            character.size       = { face->glyph->bitmap.width, face->glyph->bitmap.rows };
            character.bearing    = { face->glyph->bitmap_left, face->glyph->bitmap_top };
            character.advance    = face->glyph->advance.x;

            width_max = character.size.x > width_max ? character.size.x : width_max;
            m_characters.insert( std::pair< char, sCharacter >( c, character ) );

            glTextureSubImage3D( m_bitmap->getTexture(), 0, 0, 0, c, character.size.x, character.size.y, 1, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer );
        }

        m_bitmap->unbind();
        FT_Done_Face( face );
        FT_Done_FreeType( library );

        m_vertices[ 0 ] = { glm::vec3( width_max, pixel_height, 0 ), glm::vec2( 1, 1 ) };
        m_vertices[ 1 ] = { glm::vec3( width_max, 0, 0 ), glm::vec2( 1, 0 ) };
        m_vertices[ 2 ] = { glm::vec3( 0, 0, 0 ), glm::vec2( 0, 0 ) };
        m_vertices[ 3 ] = { glm::vec3( 0, pixel_height, 0 ), glm::vec2( 0, 1 ) };

        glBindVertexArray( vertex_array_object );

        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof( m_vertices ), m_vertices, GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ebo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( m_indices ), m_indices, GL_STATIC_DRAW );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( *m_vertices ), nullptr );
        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( *m_vertices ), reinterpret_cast< void* >( sizeof( m_vertices->position ) ) );
        glEnableVertexAttribArray( 1 );

        glBindVertexArray( 0 );
        return true;
    }

    void cFont::render( const glm::vec3& _position, const std::string& _text, const glm::vec2& _scale, const cColor& _color )
    {
        transform.local      = scale( glm::mat4( 1 ), glm::vec3( _scale, 1 ) );
        transform.local[ 3 ] = glm::vec4( _position, 1 );
        transform.update();

        color       = _color;
        latest_text = _text;
    }
}
