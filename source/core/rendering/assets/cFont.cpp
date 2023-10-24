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

        if( !m_bitmap )
            m_bitmap = MEMORY_ALLOC( cTexture, 1, GL_TEXTURE_2D );

        m_bitmap->bind();
        m_bitmap->setPixelStoreI( GL_UNPACK_ALIGNMENT, 1 );
        m_bitmap->setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        m_bitmap->setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        m_bitmap->setTextureParameterI( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        m_bitmap->setTextureParameterI( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        m_bitmap->unbind();

        unsigned width_offset = 0;
        for( unsigned char c = 0; c < 128; ++c )
        {
            if( FT_Load_Char( face, c, FT_LOAD_RENDER ) )
            {
                LOG_ERROR( std::format( "Failed to load glyph: {}", c ) );
                continue;
            }

            const FT_GlyphSlot glyph = face->glyph;
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, reinterpret_cast< int& >( glyph->bitmap.width ), reinterpret_cast< int& >( glyph->bitmap.rows ), 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer );

            sCharacter character   = {};
            character.width_offset = width_offset;
            character.size         = { face->glyph->bitmap.width, face->glyph->bitmap.rows };
            character.bearing      = { face->glyph->bitmap_left, face->glyph->bitmap_top };
            character.advance      = face->glyph->advance.x;

            width_offset += character.size.x;
            m_characters.insert( std::pair< char, sCharacter >( c, character ) );
        }

        FT_Done_Face( face );
        FT_Done_FreeType( library );
        return true;
    }
}
