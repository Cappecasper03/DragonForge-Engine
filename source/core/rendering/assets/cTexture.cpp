#include "cTexture.h"

#include <format>
#include <stb_image.h>
#include <glad/glad.h>

#include "core/log/Log.h"

namespace df
{
    cTexture::cTexture( const int& _target, const std::string& _file, const int& _mipmaps, const bool& _generate_mipmaps )
    : m_target( _target )
    {
        glGenTextures( 1, &m_texture );

        if( _file.empty() )
            return;

        bind();
        load( _file, _mipmaps, _generate_mipmaps );
        unbind();
    }

    cTexture::~cTexture()
    {
        glDeleteTextures( 1, &m_texture );
    }

    bool cTexture::load( const std::string& _file, const int& _mipmaps, const bool& _generate_mipmaps )
    {
        int            width, height, nr_channels;
        unsigned char* data = stbi_load( _file.data(), &width, &height, &nr_channels, 0 );

        if( !data )
        {
            LOG_WARNING( std::format( "Failed to load texture: {}", _file ) );
            return false;
        }

        if( nr_channels == 3 )
            glTexImage2D( m_target, _mipmaps, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
        else if( nr_channels == 4 )
            glTexImage2D( m_target, _mipmaps, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

        if( _generate_mipmaps )
            glGenerateMipmap( m_target );

        stbi_image_free( data );
        m_path = _file;
        return true;
    }

    void cTexture::setTextureParameterI( const int& _name, const int& _param ) const
    {
        glTexParameteri( m_target, _name, _param );
    }

    void cTexture::setPixelStoreI( const int& _name, const int& _param ) const
    {
        glPixelStorei( _name, _param );
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
