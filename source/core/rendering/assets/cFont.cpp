#include "cFont.h"

#include <format>
#include <freetype/freetype.h>
#include <glad/glad.h>

#include "cTexture.h"
#include "core/log/Log.h"
#include "core/memory/Memory.h"
#include "core/rendering/cShader.h"
#include "core/rendering/callbacks/DefaultFontCB.h"

namespace df
{
    cFont::cFont( const std::string& _file )
    : render_callback( nullptr ),
      color( color::white ),
      m_texture_array( nullptr )
    {
        glGenVertexArrays( 1, &vertex_array_object );
        glGenBuffers( 1, &vertex_buffer_object );
        glGenBuffers( 1, &m_ebo );

        glBindVertexArray( vertex_array_object );

        glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer_object );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 6 * 4, nullptr, GL_DYNAMIC_DRAW );
        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), nullptr );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        glBindVertexArray( 0 );

        if( !_file.empty() )
            load( _file );
    }

    cFont::~cFont()
    {
        glDeleteBuffers( 1, &m_ebo );
        glDeleteBuffers( 1, &vertex_buffer_object );
        glDeleteVertexArrays( 1, &vertex_array_object );
    }

    void cFont::load( const std::string& _file )
    {
        FT_Library library;
        if( FT_Init_FreeType( &library ) )
        {
            LOG_ERROR( "Failed to initialize FreeType library" );
            return;
        }

        FT_Face face;
        if( FT_New_Face( library, _file.data(), 0, &face ) )
        {
            LOG_ERROR( "Failed to load font" );
            return;
        }

        FT_Set_Pixel_Sizes( face, 0, 48 );

        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

        if( m_texture_array )
            MEMORY_FREE( m_texture_array );
        m_texture_array = MEMORY_ALLOC( cTexture, 1, GL_TEXTURE_2D_ARRAY );

        m_texture_array->bind();
        m_texture_array->setTextureParameterI( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        m_texture_array->setTextureParameterI( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        m_texture_array->setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        m_texture_array->setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RED, 60, 48, 95, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr );

        for( unsigned char c = 32; c < 127; ++c )
        {
            if( FT_Load_Char( face, c, FT_LOAD_RENDER ) )
            {
                LOG_ERROR( std::format( "Failed to load glyph: {}", c ) );
                continue;
            }

            sCharacter character = {};
            character.scale      = glm::vec2( static_cast< float >( face->glyph->bitmap.width ) / 60.f, static_cast< float >( face->glyph->bitmap.rows ) / 48.f );
            character.size       = glm::ivec2( face->glyph->bitmap.width, face->glyph->bitmap.rows );
            character.bearing    = glm::ivec2( face->glyph->bitmap_left, face->glyph->bitmap_top );
            character.advance    = face->glyph->advance.x;

            glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 0, 0, c - 32, character.size.x, character.size.y, 1, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer );

            characters.insert( std::pair< char, sCharacter >( c, character ) );
        }

        m_texture_array->unbind();
        FT_Done_Face( face );
        FT_Done_FreeType( library );
    }

    void cFont::render( const std::string& _text, const glm::vec3& _position, const glm::vec2& _scale, const cColor& _color )
    {
        text     = _text;
        position = _position;
        scale    = _scale;
        color    = _color;

        render_callback::defaultFont( &shader, this );
    }

    void cFont::bindTexture( const int& _index ) const
    {
        if( m_texture_array )
            m_texture_array->bind( _index );
    }
}
