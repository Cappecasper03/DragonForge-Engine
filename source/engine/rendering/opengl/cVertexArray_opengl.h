#pragma once

#include <glad/glad.h>

#include "engine/misc/Misc.h"
#include "OpenGlTypes.h"

namespace df::opengl
{
	class cVertexArray_opengl
	{
	public:
		DF_DisableCopyAndMove( cVertexArray_opengl );

		cVertexArray_opengl();
		~cVertexArray_opengl();

		void setAttribute( GLuint _index, GLint _size, eDataType _type, GLsizei _stride, size_t _offset, bool _enable = true ) const;

		void enableAttribute( GLuint _index ) const;

		void        bind() const;
		static void unbind();

	private:
		unsigned m_id;
	};
}
