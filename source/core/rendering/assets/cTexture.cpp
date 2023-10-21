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

        setTextureParameterI( GL_TEXTURE_WRAP_S, GL_REPEAT );
        setTextureParameterI( GL_TEXTURE_WRAP_T, GL_REPEAT );
        setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
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

        bind();
        if( nr_channels == 3 )
            glTexImage2D( m_target, _mipmaps, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
        else if( nr_channels == 4 )
            glTexImage2D( m_target, _mipmaps, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

        if( _generate_mipmaps )
            glGenerateMipmap( m_target );

        unbind();
        stbi_image_free( data );
    }

    void cTexture::setTextureParameterI( const int& _name, const int& _param ) const
    {
        bind();
        glTexParameteri( m_target, _name, _param );
        unbind();
    }

    void cTexture::bind() const
    {
        glBindTexture( m_target, m_texture );
    }

    void cTexture::unbind() const
    {
        glBindTexture( m_target, 0 );
    }
}
