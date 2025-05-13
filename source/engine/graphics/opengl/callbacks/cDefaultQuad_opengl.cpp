#include "cDefaultQuad_opengl.h"

#include "engine/core/utils/cTransform.h"
#include "engine/graphics/api/iRenderer.h"
#include "engine/graphics/assets/iTexture.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/cFramebuffer_opengl.h"
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
			.world_matrix = _quad->m_transform->m_world,
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
			.world_matrix = _quad->m_transform->m_world,
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

		const cFramebuffer_opengl* render_framebuffer = reinterpret_cast< const cFramebuffer_opengl* >( cRenderer::getRenderInstance()->getDeferredFramebuffer() );

		_shader->use();

		const iQuad::sPushConstants push_constants{
			.world_matrix = _quad->m_transform->m_world,
		};

		_quad->m_push_constant.bind();
		_quad->m_push_constant.setSubData( 0, sizeof( iQuad::sPushConstants ), &push_constants );
		_quad->m_push_constant.unbind();
		_quad->m_push_constant.bindBase( 0 );

		render_framebuffer->m_render_textures[ 0 ]->bind( 0 );
		render_framebuffer->m_render_textures[ 1 ]->bind( 1 );
		render_framebuffer->m_render_textures[ 2 ]->bind( 2 );

		glEnable( kDepthTest );

		_quad->m_vertex_array.bind();
		glDrawElements( kTriangles, 6, kUnsignedInt, nullptr );

		glDisable( kDepthTest );
	}
}