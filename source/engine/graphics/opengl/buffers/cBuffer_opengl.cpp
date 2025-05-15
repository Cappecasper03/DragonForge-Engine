#include "cBuffer_opengl.h"

#include <glad/glad.h>

#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cBuffer_opengl::cBuffer_opengl( const eType _type, const bool _generate )
		: m_type( _type )
	{
		DF_ProfilingScopeCpu;

		if( _generate )
			generate();
	}

	cBuffer_opengl::~cBuffer_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteBuffers( 1, &m_id );
	}

	void cBuffer_opengl::generate()
	{
		DF_ProfilingScopeCpu;

		glGenBuffers( 1, &m_id );
	}

	void cBuffer_opengl::setData( const GLsizeiptr _size, const void* _data, const eUsage _usage ) const
	{
		DF_ProfilingScopeCpu;

		glBufferData( m_type, _size, _data, _usage );
	}

	void cBuffer_opengl::setSubData( const GLsizeiptr _offset, const GLsizeiptr _size, const void* _data ) const
	{
		DF_ProfilingScopeCpu;

		glBufferSubData( m_type, _offset, _size, _data );
	}

	void cBuffer_opengl::bind() const
	{
		DF_ProfilingScopeCpu;

		glBindBuffer( m_type, m_id );
	}

	void cBuffer_opengl::bindBase( const unsigned _index ) const
	{
		DF_ProfilingScopeCpu;

		glBindBufferBase( m_type, _index, m_id );
	}

	void cBuffer_opengl::unbind() const
	{
		DF_ProfilingScopeCpu;

		glBindBuffer( m_type, 0 );
	}
}