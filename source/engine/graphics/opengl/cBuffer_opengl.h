#pragma once

#include <glad/glad.h>

#include "engine/core/utils/Misc.h"

namespace df::opengl
{
	class cBuffer_opengl
	{
	public:
		DF_DisableCopyAndMove( cBuffer_opengl );

		enum eType
		{
			kVertex  = GL_ARRAY_BUFFER,
			kIndex   = GL_ELEMENT_ARRAY_BUFFER,
			kUniform = GL_UNIFORM_BUFFER,
		};

		enum eUsage
		{
			kStreamDraw  = GL_STREAM_DRAW,
			kStreamRead  = GL_STREAM_READ,
			kStreamCopy  = GL_STREAM_COPY,
			kStaticDraw  = GL_STATIC_DRAW,
			kStaticRead  = GL_STATIC_READ,
			kStaticCopy  = GL_STATIC_COPY,
			kDynamicDraw = GL_DYNAMIC_DRAW,
			kDynamicRead = GL_DYNAMIC_READ,
			kDynamicCopy = GL_DYNAMIC_COPY,
		};

		cBuffer_opengl( eType _type, bool _generate = true );
		~cBuffer_opengl();

		void generate();

		void setData( GLsizeiptr _size, const void* _data, eUsage _usage ) const;
		void setSubData( GLsizeiptr _offset, GLsizeiptr _size, const void* _data ) const;

		void bind() const;
		void bindBase( unsigned _index = 0 ) const;
		void unbind() const;

	private:
		eType    m_type;
		unsigned m_id;
	};
}
