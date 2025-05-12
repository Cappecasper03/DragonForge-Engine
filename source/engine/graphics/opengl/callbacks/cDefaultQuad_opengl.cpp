#include "cDefaultQuad_opengl.h"

#include "engine/core/utils/cTransform.h"
#include "engine/graphics/api/iRenderer.h"
#include "engine/graphics/assets/iTexture.h"
#include "engine/graphics/cameras/cCamera.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/cFramebuffer_opengl.h"
#include "engine/managers/cCameraManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_opengl.h"

namespace df::opengl::render_callbacks
{
	void cDefaultQuad_opengl::forwardQuad( const cShader_opengl* _shader, const cQuad_opengl* _quad )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		const cCamera* camera = cCameraManager::getInstance()->current;

		_shader->use();

		_shader->setUniformMatrix4F( "u_world_matrix", _quad->transform->world );
		_shader->setUniformMatrix4F( "u_view_projection_matrix", camera->view_projection );

		_shader->setUniform4F( "u_color", _quad->color );

		_shader->setUniformSampler( "u_texture", 0 );

		if( _quad->texture )
			_quad->texture->bind();

		glEnable( kDepthTest );

		_quad->vertex_array.bind();
		glDrawElements( kTriangles, 6, kUnsignedInt, nullptr );

		glDisable( kDepthTest );
	}

	void cDefaultQuad_opengl::deferredQuad( const cShader_opengl* _shader, const cQuad_opengl* _quad )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		const cCamera* camera = cCameraManager::getInstance()->current;

		_shader->use();

		_shader->setUniformMatrix4F( "u_world_matrix", _quad->transform->world );
		_shader->setUniformMatrix4F( "u_view_projection_matrix", camera->view_projection );

		_shader->setUniform4F( "u_color", _quad->color );

		_shader->setUniformSampler( "u_texture", 0 );

		if( _quad->texture )
			_quad->texture->bind();

		glEnable( kDepthTest );

		_quad->vertex_array.bind();
		glDrawElements( kTriangles, static_cast< GLsizei >( _quad->getIndices().size() ), kUnsignedInt, nullptr );

		glDisable( kDepthTest );
	}

	void cDefaultQuad_opengl::deferredQuadFinal( const cShader_opengl* _shader, const cQuad_opengl* _quad )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		const cFramebuffer_opengl* render_framebuffer = reinterpret_cast< const cFramebuffer_opengl* >( cRenderer::getRenderInstance()->getDeferredFramebuffer() );
		const cCamera*             camera             = cCameraManager::getInstance()->current;

		_shader->use();

		_shader->setUniformMatrix4F( "u_world_matrix", _quad->transform->world );
		_shader->setUniformMatrix4F( "u_view_projection_matrix", camera->view_projection );

		_shader->setUniformSampler( "u_position_texture", 0 );
		render_framebuffer->render_textues[ 0 ]->bind( 0 );

		_shader->setUniformSampler( "u_normal_texture", 1 );
		render_framebuffer->render_textues[ 1 ]->bind( 1 );

		_shader->setUniformSampler( "u_color_specular_texture", 2 );
		render_framebuffer->render_textues[ 2 ]->bind( 2 );

		glEnable( kDepthTest );

		_quad->vertex_array.bind();
		glDrawElements( kTriangles, 6, kUnsignedInt, nullptr );

		glDisable( kDepthTest );
	}
}