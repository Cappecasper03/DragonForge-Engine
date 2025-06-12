#pragma once

#include "engine/graphics/cameras/cRenderTextureCamera2D.h"
#include "engine/graphics/opengl/buffers/cFrameBuffer_opengl.h"
#include "engine/graphics/opengl/buffers/cRenderBuffer_opengl.h"

namespace df::opengl
{
	class cRenderTextureCamera2D_opengl : public cRenderTextureCamera2D
	{
	public:
		DF_DeleteCopyAndMove( cRenderTextureCamera2D_opengl );

		cRenderTextureCamera2D_opengl( const sDescription& _description );
		~cRenderTextureCamera2D_opengl() override = default;

		void createTexture( const cRenderTexture2D::sDescription& _description ) override;

		void beginRender( int _clear_buffers ) override;
		void endRender() override;

	private:
		cFrameBuffer_opengl  m_frame_buffer;
		cRenderBuffer_opengl m_render_buffer;
	};
}
