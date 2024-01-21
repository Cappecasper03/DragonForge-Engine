#include "cFramebuffer.h"

#include <format>
#include <glad/glad.h>

#include "cRenderer.h"
#include "assets/cTexture.h"

#if PROFILING
#include "core/profiling/Profiling.h"
#endif

namespace df
{
    cFramebuffer::cFramebuffer( std::string _name, const unsigned& _num_render_textures, const bool& _generate_render_buffer )
    : render_textues( _num_render_textures, nullptr ),
      name( std::move( _name ) )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glGenFramebuffers( 1, &m_buffer );
        bind();
        const glm::ivec2& window_size = cRenderer::getWindowSize();

        if( _generate_render_buffer )
        {
            glGenRenderbuffers( 1, &m_render_buffer );
            glBindRenderbuffer( GL_RENDERBUFFER, m_render_buffer );
            glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_STENCIL, window_size.x, window_size.y );
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer );
        }

        if( render_textues.empty() )
            return;

        std::vector< unsigned > texture_attachments{};
        for( int i = 0; i < render_textues.size(); ++i )
        {
            cTexture* texture = new cTexture( std::format( "{}_{}", name, i ), GL_TEXTURE_2D );

            texture->bind();
            texture->setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            texture->setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            texture->setTexImage2D( 0, GL_RGBA, window_size.x, window_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->getTexture(), 0 );

            texture_attachments.push_back( GL_COLOR_ATTACHMENT0 + static_cast< unsigned >( i ) );
            render_textues[ i ] = texture;
        }

        glDrawBuffers( static_cast< GLsizei >( texture_attachments.size() ), texture_attachments.data() );
        unbind();
    }

    cFramebuffer::~cFramebuffer()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        for( const cTexture* render_textue : render_textues )
            delete render_textue;

        glDeleteFramebuffers( 1, &m_buffer );
    }

    void cFramebuffer::setFramebufferTexture2D( const int _attachment, const int _tex_target, const int _texture, const int _level )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glFramebufferTexture2D( GL_RENDERBUFFER, _attachment, _tex_target, _texture, _level );
    }

    void cFramebuffer::bind() const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glBindFramebuffer( GL_FRAMEBUFFER, m_buffer );
    }

    void cFramebuffer::unbind()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }
}
