#include "cTexture.h"

#include <format>
#include <stb_image.h>
#include <glad/glad.h>

#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"

namespace df
{
    cTexture::cTexture( std::string _name, const int& _target, const std::string& _file, const int& _mipmaps, const bool& _generate_mipmaps, const bool& _flip_vertically_on_load )
    : name( std::move( _name ) ),
      m_target( _target )
    {
        glGenTextures( 1, &m_texture );

        if( _file.empty() )
            return;

        load( _file, _mipmaps, _generate_mipmaps, _flip_vertically_on_load );
    }

    cTexture::~cTexture()
    {
        glDeleteTextures( 1, &m_texture );
    }

    bool cTexture::load( const std::string& _file, const int& _mipmaps, const bool& _generate_mipmaps, const bool& _flip_vertically_on_load )
    {
        stbi_set_flip_vertically_on_load( _flip_vertically_on_load );
        int            width, height, nr_channels;
        unsigned char* data = stbi_load( filesystem::getPath( _file ).data(), &width, &height, &nr_channels, 0 );

        if( !data )
        {
            LOG_WARNING( std::format( "Failed to load texture: {}", _file ) );
            unbind();
            return false;
        }

        bind();
        if( nr_channels == 3 )
            setTexImage2D( _mipmaps, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
        else if( nr_channels == 4 )
            setTexImage2D( _mipmaps, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

        if( _generate_mipmaps )
            glGenerateMipmap( m_target );

        stbi_image_free( data );
        m_path = _file;
        unbind();
        return true;
    }

    void cTexture::setTexImage2D( const int _level, const int _internal_format, const int _width, const int _height, const int _border, const unsigned _format, const unsigned _type, const void* _pixels ) const
    {
        glTexImage2D( m_target, _level, _internal_format, _width, _height, _border, _format, _type, _pixels );
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
    }

    void cTexture::unbind( const int& _index ) const
    {
        glActiveTexture( GL_TEXTURE0 + _index );
        glBindTexture( m_target, 0 );
    }
}
