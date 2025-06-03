#pragma once

#include "cRenderBuffer_opengl.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/api/iFramebuffer.h"

namespace df::opengl
{
	class cTexture2D_opengl;

	class cFrameBuffer_opengl final : public iFramebuffer
	{
	public:
		DF_DeleteCopyAndMove( cFrameBuffer_opengl );

		cFrameBuffer_opengl();
		~cFrameBuffer_opengl() override;

		void setRenderBuffer( int _attachment, const cRenderBuffer_opengl& _render_buffer ) const;

		void setTexture2D( int _attachment_index, const cTexture2D_opengl* _texture ) const;
		void setTexture2D( int _attachment_index, const cTexture2D_opengl& _texture ) const;

		void bind() override;
		void unbind() override;

	private:
		unsigned m_buffer;
	};
}
