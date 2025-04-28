#include "cBuffer_opengl.h"

#include <glad/glad.h>

#include "profiling/ProfilingMacros.h"

namespace df::opengl
{
	cBuffer_opengl::cBuffer_opengl( const eType _type )
		: m_type( _type )
	{
		DF_ProfilingScopeCpu;

		glGenBuffers( 1, &m_id );
	}

	cBuffer_opengl::~cBuffer_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteBuffers( 1, &m_id );
	}

	void cBuffer_opengl::setData( const GLsizeiptr _size, const void* _data, const GLenum _usage ) const
	{
		DF_ProfilingScopeCpu;

		glNamedBufferData( m_id, _size, _data, _usage );
	}

	void cBuffer_opengl::bind() const
	{
		DF_ProfilingScopeCpu;

		glBindBuffer( m_type, m_id );
	}

	void cBuffer_opengl::unbind() const
	{
		DF_ProfilingScopeCpu;

		glBindBuffer( m_type, 0 );
	}
}