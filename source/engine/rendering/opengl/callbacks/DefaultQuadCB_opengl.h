#pragma once

#include <glad/glad.h>

#include "engine/managers/assets/cCameraManager.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/iRenderer.h"
#include "engine/rendering/assets/iTexture.h"
#include "engine/rendering/opengl/cFramebuffer_opengl.h"
#include "engine/rendering/OpenGL/cShader_opengl.h"
#include "engine/rendering/opengl/assets/cQuad_opengl.h"

namespace df::opengl::render_callback
{
	inline void defaultQuad( const cShader_opengl* _shader, const cQuad_opengl* _quad )
	{
		ZoneScoped;

		const cCamera* camera = cCameraManager::getInstance()->current;

		_shader->use();

		_shader->setUniformMatrix4F( "u_world_matrix", _quad->transform->world );
		_shader->setUniformMatrix4F( "u_projection_view_matrix", camera->projection_view );

		_shader->setUniform1B( "u_use_texture", _quad->texture );
		_shader->setUniform4F( "u_color", _quad->color );

		_shader->setUniformSampler( "u_texture", 0 );
		_quad->texture->bind();

		glEnable( GL_DEPTH_TEST );
		glBindVertexArray( _quad->vertex_array );

		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );

		glDisable( GL_DEPTH_TEST );
	}

	inline void defaultQuadDeferred( const cShader_opengl* _shader, const cQuad_opengl* _quad )
	{
		ZoneScoped;

		const cFramebuffer_opengl* render_framebuffer = reinterpret_cast< const cFramebuffer_opengl* >( cRenderer::getRenderInstance()->getFramebuffer() );
		const cCamera*             camera             = cCameraManager::getInstance()->current;

		_shader->use();

		_shader->setUniformMatrix4F( "u_world_matrix", _quad->transform->world );
		_shader->setUniformMatrix4F( "u_projection_view_matrix", camera->projection_view );

		_shader->setUniformSampler( "u_position_texture", 0 );
		render_framebuffer->render_textues[ 0 ]->bind( 0 );

		_shader->setUniformSampler( "u_normal_texture", 1 );
		render_framebuffer->render_textues[ 1 ]->bind( 1 );

		_shader->setUniformSampler( "u_color_specular_texture", 2 );
		render_framebuffer->render_textues[ 2 ]->bind( 2 );

		glEnable( GL_DEPTH_TEST );
		glBindVertexArray( _quad->vertex_array );

		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr );

		glDisable( GL_DEPTH_TEST );
	}
}
