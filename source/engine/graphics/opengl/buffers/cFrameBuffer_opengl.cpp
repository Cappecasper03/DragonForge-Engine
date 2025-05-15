#include "cFrameBuffer_opengl.h"

#include <glad/glad.h>

#include "engine/graphics/opengl/assets/cTexture_opengl.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cFrameBuffer_opengl::cFrameBuffer_opengl()
	{
		DF_ProfilingScopeCpu;

		glGenFramebuffers( 1, &m_buffer );
	}

	cFrameBuffer_opengl::~cFrameBuffer_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteFramebuffers( 1, &m_buffer );
	}

	void cFrameBuffer_opengl::setRenderBuffer( const int _attachment, const cRenderBuffer_opengl& _render_buffer ) const
	{
		DF_ProfilingScopeCpu;

		glFramebufferRenderbuffer( GL_FRAMEBUFFER, _attachment, GL_RENDERBUFFER, _render_buffer.get() );
	}

	void cFrameBuffer_opengl::setTexture2D( const int _attachment_index, const cTexture_opengl* _texture ) const
	{
		DF_ProfilingScopeCpu;

		glFramebufferTexture2D( GL_FRAMEBUFFER, _attachment_index, GL_TEXTURE_2D, _texture->get(), 0 );
	}

	void cFrameBuffer_opengl::setTexture2D( const int _attachment_index, const cTexture_opengl& _texture ) const
	{
		DF_ProfilingScopeCpu;

		glFramebufferTexture2D( GL_FRAMEBUFFER, _attachment_index, GL_TEXTURE_2D, _texture.get(), 0 );
	}

	void cFrameBuffer_opengl::bind()
	{
		DF_ProfilingScopeCpu;

		glBindFramebuffer( GL_FRAMEBUFFER, m_buffer );
	}

	void cFrameBuffer_opengl::unbind()
	{
		DF_ProfilingScopeCpu;

		glBindRenderbuffer( GL_FRAMEBUFFER, 0 );
	}
}