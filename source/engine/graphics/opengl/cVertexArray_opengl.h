#pragma once

#include <cstddef>
#include <glad/glad.h>

#include "engine/core/utils/Misc.h"
#include "OpenGlTypes.h"

namespace df::opengl
{
	class cVertexArray_opengl
	{
	public:
		DF_DeleteCopy( cVertexArray_opengl );
		DF_DefaultMove( cVertexArray_opengl );

		cVertexArray_opengl( bool _generate = true );
		~cVertexArray_opengl();

		void generate();

		void setAttribute( GLuint _index, GLint _size, eDataType _type, GLsizei _stride, std::size_t _offset, bool _enable = true, GLboolean _normalized = false ) const;

		void enableAttribute( GLuint _index ) const;

		void        bind() const;
		static void unbind();

	private:
		unsigned m_id;
	};
}
