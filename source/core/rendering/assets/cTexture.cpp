#include "cTexture.h"

#include <format>
#include <stb_image.h>
#include <glad/glad.h>

#include "core/log/Log.h"

namespace df
{
    cTexture::cTexture( const int& _target )
    : m_target( _target )
    {
        glGenTextures( 1, &m_texture );

        bind( 0 );
        setTextureParameterI( GL_TEXTURE_WRAP_S, GL_REPEAT );
        setTextureParameterI( GL_TEXTURE_WRAP_T, GL_REPEAT );
        setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        unbind( 0 );
    }

    cTexture::~cTexture()
    {
        glDeleteTextures( 1, &m_texture );
    }

    void cTexture::load( const std::string& _file, const int& _mipmaps, const bool& _generate_mipmaps ) const
    {
        int            width, height, nr_channels;
        unsigned char* data = stbi_load( _file.data(), &width, &height, &nr_channels, 0 );

        if( !data )
        {
            LOG_WARNING( std::format( "Failed to load texture: {}", _file ) );
            return;
        }

        bind( 0 );
        if( nr_channels == 3 )
            glTexImage2D( m_target, _mipmaps, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
        else if( nr_channels == 4 )
            glTexImage2D( m_target, _mipmaps, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

        if( _generate_mipmaps )
            glGenerateMipmap( m_target );
        unbind( 0 );

        stbi_image_free( data );
    }

    void cTexture::setTextureParameterI( const int& _name, const int& _param ) const
    {
        bind( 0 );
        glTexParameteri( m_target, _name, _param );
        unbind( 0 );
    }

    void cTexture::bind( const int& _index ) const
    {
        glActiveTexture( GL_TEXTURE0 + _index );
        glBindTexture( m_target, m_texture );
        glActiveTexture( GL_TEXTURE0 );
    }

    void cTexture::unbind( const int& _index ) const
    {
        glActiveTexture( GL_TEXTURE0 + _index );
        glBindTexture( m_target, 0 );
        glActiveTexture( GL_TEXTURE0 );
    }
}
