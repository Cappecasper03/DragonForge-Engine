#pragma once

#include <glad/glad.h>

#include "engine/misc/Misc.h"

namespace df::opengl
{
	class cBuffer_opengl
	{
	public:
		DF_DisableCopyAndMove( cBuffer_opengl );

		enum eType
		{
			kVertexBuffer = 0x8892,
			kIndexBuffer  = 0x8893,
		};

		cBuffer_opengl( eType _type );
		~cBuffer_opengl();

		void setData( GLsizeiptr _size, const void* _data, GLenum _usage ) const;

		void bind() const;
		void unbind() const;

	private:
		eType    m_type;
		unsigned m_id;
	};
}
