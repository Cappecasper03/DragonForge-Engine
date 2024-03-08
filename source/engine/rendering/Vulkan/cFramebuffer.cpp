#include "cFramebuffer.h"

#include <format>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <tracy/Tracy.hpp>

#include "assets/cTexture.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/iRenderer.h"

namespace df::vulkan
{
	cFramebuffer::cFramebuffer( std::string _name, const unsigned _num_render_textures, const bool _generate_render_buffer ) : iFramebuffer( std::move( _name ) )
	{
		ZoneScoped;

		glGenFramebuffers( 1, &m_buffer );
		cFramebuffer::bind();
		const glm::ivec2& window_size = cRendererSingleton::getRenderInstance()->getWindowSize();

		if( _generate_render_buffer )
		{
			glGenRenderbuffers( 1, &m_render_buffer );
			glBindRenderbuffer( GL_RENDERBUFFER, m_render_buffer );
			glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_STENCIL, window_size.x, window_size.y );
			glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer );
		}

		std::vector< unsigned > texture_attachments{};
		for( unsigned i = 0; i < _num_render_textures; ++i )
		{
			cTexture* texture = new cTexture( "", GL_TEXTURE_2D );

			texture->bind();
			texture->setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			texture->setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			texture->setTexImage2D( 0, GL_RGBA, window_size.x, window_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );
			glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->getTexture(), 0 );

			texture_attachments.push_back( GL_COLOR_ATTACHMENT0 + i );
			render_textues.push_back( texture );
		}

		glDrawBuffers( static_cast< GLsizei >( texture_attachments.size() ), texture_attachments.data() );
		cFramebuffer::unbind();
	}

	cFramebuffer::~cFramebuffer()
	{
		ZoneScoped;

		glDeleteFramebuffers( 1, &m_buffer );
	}

	void cFramebuffer::setFramebufferTexture2D( const int _attachment, const int _tex_target, const int _texture, const int _level )
	{
		ZoneScoped;

		glFramebufferTexture2D( GL_RENDERBUFFER, _attachment, _tex_target, _texture, _level );
	}

	void cFramebuffer::bind()
	{
		ZoneScoped;

		glBindFramebuffer( GL_FRAMEBUFFER, m_buffer );
	}

	void cFramebuffer::unbind()
	{
		ZoneScoped;

		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}
}
