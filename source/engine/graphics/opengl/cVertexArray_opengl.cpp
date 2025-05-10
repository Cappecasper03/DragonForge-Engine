#include "cVertexArray_opengl.h"

#include <glad/glad.h>

#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cVertexArray_opengl::cVertexArray_opengl()
	{
		DF_ProfilingScopeCpu;

		glGenVertexArrays( 1, &m_id );
	}

	cVertexArray_opengl::~cVertexArray_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteVertexArrays( 1, &m_id );
	}

	void cVertexArray_opengl::setAttribute( const GLuint _index, const GLint _size, const eDataType _type, const GLsizei _stride, const size_t _offset, const bool _enable ) const
	{
		DF_ProfilingScopeCpu;

		glVertexAttribPointer( _index, _size, _type, false, _stride, reinterpret_cast< void* >( _offset ) );

		if( _enable )
			enableAttribute( _index );
	}

	void cVertexArray_opengl::enableAttribute( const GLuint _index ) const
	{
		DF_ProfilingScopeCpu;

		glEnableVertexAttribArray( _index );
	}

	void cVertexArray_opengl::bind() const
	{
		DF_ProfilingScopeCpu;

		glBindVertexArray( m_id );
	}

	void cVertexArray_opengl::unbind()
	{
		DF_ProfilingScopeCpu;

		glBindVertexArray( 0 );
	}
}