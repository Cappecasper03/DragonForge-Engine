#include "cFont.h"

#include <format>
#include <freetype/freetype.h>
#include <glad/glad.h>

#include "core/memory/Memory.h"

namespace df
{
    cFont::cFont( const std::string& _font_file )
    : m_bitmap( nullptr )
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

        FT_Set_Pixel_Sizes( face, 0, 48 );

        if( m_bitmap )
        {
            MEMORY_FREE( m_bitmap );
            m_bitmap = MEMORY_ALLOC( cTexture, 1, GL_TEXTURE_2D_ARRAY );
        }
        else
            m_bitmap = MEMORY_ALLOC( cTexture, 1, GL_TEXTURE_2D_ARRAY );

        m_bitmap->bind();
        m_bitmap->setPixelStoreI( GL_UNPACK_ALIGNMENT, 1 );
        m_bitmap->setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        m_bitmap->setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        m_bitmap->setTextureParameterI( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        m_bitmap->setTextureParameterI( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

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

            m_characters.insert( std::pair< char, sCharacter >( c, character ) );

            glTextureSubImage3D( m_bitmap->getTexture(), 0, 0, 0, c, character.size.y, character.size.y, 1, GL_RG, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer );
        }

        m_bitmap->unbind();
        FT_Done_Face( face );
        FT_Done_FreeType( library );
        return true;
    }

    void cFont::render( const std::string& _text )
    {
        for( char c : _text )
        {
            const sCharacter& character = m_characters[ c ];
        }
    }
}
