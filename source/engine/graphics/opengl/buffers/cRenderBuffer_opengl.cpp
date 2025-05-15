#include "cRenderBuffer_opengl.h"

#include <glad/glad.h>

#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cRenderBuffer_opengl::cRenderBuffer_opengl()
	{
		DF_ProfilingScopeCpu;

		glGenRenderbuffers( 1, &m_buffer );
	}

	cRenderBuffer_opengl::~cRenderBuffer_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteRenderbuffers( 1, &m_buffer );
	}

	void cRenderBuffer_opengl::setStorage( const int _format, const cVector2i& _size ) const
	{
		DF_ProfilingScopeCpu;

		glRenderbufferStorage( GL_RENDERBUFFER, _format, _size.x(), _size.y() );
	}

	void cRenderBuffer_opengl::bind() const
	{
		DF_ProfilingScopeCpu;

		glBindRenderbuffer( GL_RENDERBUFFER, m_buffer );
	}

	void cRenderBuffer_opengl::unbind() const
	{
		DF_ProfilingScopeCpu;

		glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	}
}