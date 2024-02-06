#include "cFont.h"

#include <format>
#include <freetype/freetype.h>
#include <glad/glad.h>

#include "cTexture.h"
#include "engine/log/Log.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/managers/assets/cFontManager.h"
#include "engine/rendering/cShader.h"

namespace df
{
    cFont::cFont( std::string _name, const std::string& _file )
    : iAsset( std::move( _name ) ),
      render_callback( nullptr ),
      m_texture_array( nullptr ),
      m_latest_color( color::white )
    {
        glGenVertexArrays( 1, &vertex_array_object );
        glGenBuffers( 1, &vertex_buffer_object );
        glGenBuffers( 1, &m_ebo );

        glBindVertexArray( vertex_array_object );

        glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer_object );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 6 * 5, nullptr, GL_DYNAMIC_DRAW );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), nullptr );
        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), reinterpret_cast< void* >( 3 * sizeof( float ) ) );
        glEnableVertexAttribArray( 1 );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindVertexArray( 0 );

        if( !_file.empty() )
            load( _file );
    }

    cFont::~cFont()
    {
        delete m_texture_array;

        glDeleteBuffers( 1, &m_ebo );
        glDeleteBuffers( 1, &vertex_buffer_object );
        glDeleteVertexArrays( 1, &vertex_array_object );
    }

    bool cFont::load( const std::string& _file )
    {
        FT_Library library;
        if( FT_Init_FreeType( &library ) )
        {
            LOG_ERROR( "Failed to initialize FreeType library" );
            return false;
        }

        FT_Face face;
        if( FT_New_Face( library, _file.data(), 0, &face ) )
        {
            LOG_ERROR( "Failed to load font" );
            return false;
        }

        FT_Set_Pixel_Sizes( face, 0, 48 );

        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

        delete m_texture_array;
        m_texture_array = new cTexture( std::format( "{}_font", name ), GL_TEXTURE_2D_ARRAY );

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

            m_characters.insert( std::pair< char, sCharacter >( c, character ) );
        }

        m_texture_array->unbind();
        FT_Done_Face( face );
        FT_Done_FreeType( library );
        return true;
    }

    void cFont::render( const std::string& _text, const glm::vec3& _position, const glm::vec2& _scale, const cColor& _color )
    {
        m_latest_text     = _text;
        m_latest_position = _position;
        m_latest_scale    = _scale;
        m_latest_color    = _color;

        if( cFontManager::getForcedRenderCallback() )
            cRenderCallbackManager::render( cFontManager::getForcedRenderCallback(), this );
        else if( render_callback )
            cRenderCallbackManager::render( render_callback, this );
        else
            cRenderCallbackManager::render( cFontManager::getDefaultRenderCallback(), this );
    }

    void cFont::bindTexture( const int& _index ) const
    {
        if( m_texture_array )
            m_texture_array->bind( _index );
    }
}
