#include "cRenderTextureCamera2D_opengl.h"

#include <glad/glad.h>

#include "engine/graphics/api/iGraphicsDevice.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/window/iWindow.h"

namespace df::opengl
{
	cRenderTextureCamera2D_opengl::cRenderTextureCamera2D_opengl( const sDescription& _description )
		: cRenderTextureCamera2D( _description )
	{
		DF_ProfilingScopeCpu;

		m_frame_buffer.bind();
		m_render_buffer.bind();
		m_render_buffer.setStorage( GL_DEPTH_STENCIL, cRenderer::getGraphicsDevice()->getWindow()->getSize() );
		m_frame_buffer.setRenderBuffer( GL_DEPTH_STENCIL_ATTACHMENT, m_render_buffer );
	}

	void cRenderTextureCamera2D_opengl::createTexture( const cRenderTexture2D::sDescription& _description )
	{
		DF_ProfilingScopeCpu;

		cRenderTextureCamera2D::createTexture( _description );

		m_frame_buffer.bind();
		m_frame_buffer.setTexture2D( m_textures.size() - 1, reinterpret_cast< const cTexture2D_opengl* >( m_textures.back() ) );
	}

	void cRenderTextureCamera2D_opengl::beginRender( const int _clear_buffers )
	{
		DF_ProfilingScopeCpu;

		m_frame_buffer.bind();

		cRenderTextureCamera2D::beginRender( _clear_buffers );
	}

	void cRenderTextureCamera2D_opengl::endRender()
	{
		DF_ProfilingScopeCpu;

		cRenderTextureCamera2D::endRender();

		m_frame_buffer.unbind();
	}
}
