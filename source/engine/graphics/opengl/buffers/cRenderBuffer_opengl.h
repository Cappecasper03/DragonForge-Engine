#pragma once

#include "engine/core/math/cVector.h"
#include "engine/core/utils/Misc.h"

namespace df::opengl
{
	class cRenderBuffer_opengl
	{
	public:
		DF_DefaultCopyAndMove( cRenderBuffer_opengl );

		cRenderBuffer_opengl();
		~cRenderBuffer_opengl();

		void setStorage( int _format, const cVector2i& _size ) const;

		void bind() const;
		void unbind() const;

		unsigned int get() const { return m_buffer; }

	private:
		unsigned m_buffer;
	};
}
