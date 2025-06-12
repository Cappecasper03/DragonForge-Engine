#include "cDefaultQuad_opengl.h"

#include "engine/core/utils/cTransform.h"
#include "engine/graphics/api/iGraphicsDevice.h"
#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/assets/textures/iTexture.h"
#include "engine/graphics/cameras/cRenderTextureCamera2D.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/buffers/cFrameBuffer_opengl.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_opengl.h"

namespace df::opengl::render_callbacks
{
	void cDefaultQuad_opengl::forwardQuad( const cShader_opengl* _shader, const cQuad_opengl* _quad )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		_shader->use();

		const iQuad::sPushConstants push_constants{
			.world_matrix = _quad->m_transform.m_world,
		};

		_quad->m_push_constant.bind();
		_quad->m_push_constant.setSubData( 0, sizeof( iQuad::sPushConstants ), &push_constants );
		_quad->m_push_constant.unbind();
		_quad->m_push_constant.bindBase( 0 );

		if( _quad->m_texture )
			_quad->m_texture->bind();

		glEnable( kDepthTest );

		_quad->m_vertex_array.bind();
		glDrawElements( kTriangles, 6, kUnsignedInt, nullptr );

		glDisable( kDepthTest );
	}

	void cDefaultQuad_opengl::deferredQuad( const cShader_opengl* _shader, const cQuad_opengl* _quad )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		_shader->use();

		const iQuad::sPushConstants push_constants{
			.world_matrix = _quad->m_transform.m_world,
		};

		_quad->m_push_constant.bind();
		_quad->m_push_constant.setSubData( 0, sizeof( iQuad::sPushConstants ), &push_constants );
		_quad->m_push_constant.unbind();
		_quad->m_push_constant.bindBase( 0 );

		if( _quad->m_texture )
			_quad->m_texture->bind();

		glEnable( kDepthTest );

		_quad->m_vertex_array.bind();
		glDrawElements( kTriangles, static_cast< GLsizei >( _quad->getIndices().size() ), kUnsignedInt, nullptr );

		glDisable( kDepthTest );
	}

	void cDefaultQuad_opengl::deferredQuadFinal( const cShader_opengl* _shader, const cQuad_opengl* _quad )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		const cRenderTextureCamera2D* deferred_camera = cRenderer::getGraphicsDevice()->getDeferredCamera();

		_shader->use();

		const iQuad::sPushConstants push_constants{
			.world_matrix = _quad->m_transform.m_world,
		};

		_quad->m_push_constant.bind();
		_quad->m_push_constant.setSubData( 0, sizeof( iQuad::sPushConstants ), &push_constants );
		_quad->m_push_constant.unbind();
		_quad->m_push_constant.bindBase( 0 );

		deferred_camera->bindTexture( 0, 0 );
		deferred_camera->bindTexture( 1, 1 );
		deferred_camera->bindTexture( 2, 2 );

		glEnable( kDepthTest );

		_quad->m_vertex_array.bind();
		glDrawElements( kTriangles, 6, kUnsignedInt, nullptr );

		glDisable( kDepthTest );
	}
}