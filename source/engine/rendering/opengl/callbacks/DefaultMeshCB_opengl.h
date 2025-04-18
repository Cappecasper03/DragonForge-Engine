﻿#pragma once

#include <glad/glad.h>

#include "engine/misc/cTransform.h"
// #include "engine/profiling/ProfilingMacros_opengl.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/rendering/assets/iTexture.h"
#include "engine/rendering/opengl/assets/cMesh_opengl.h"
#include "engine/rendering/opengl/cShader_opengl.h"

namespace df::opengl::render_callback
{
	inline void forwardMeshAmbient( const cShader_opengl* _shader, const cMesh_opengl* _mesh )
	{
		// DF_ProfilingScopeCpu;
		// DF_ProfilingScopeGpu;

		const cCamera* camera = cCameraManager::getInstance()->current;

		_shader->use();

		_shader->setUniformMatrix4F( "u_world_matrix", _mesh->transform->world );
		_shader->setUniformMatrix4F( "u_view_projection_matrix", camera->view_projection );

		_shader->setUniformSampler( "u_color_texture", 0 );
		_mesh->getTextures().at( aiTextureType_DIFFUSE )->bind();

		glEnable( GL_DEPTH_TEST );
		glEnable( GL_BLEND );

		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glBindVertexArray( _mesh->vertex_array );
		glDrawElements( GL_TRIANGLES, static_cast< GLsizei >( _mesh->getIndices().size() ), GL_UNSIGNED_INT, nullptr );

		glDisable( GL_BLEND );
		glDisable( GL_DEPTH_TEST );
	}

	inline void forwardMesh( const cShader_opengl* _shader, const cMesh_opengl* _mesh )
	{
		// DF_ProfilingScopeCpu;

		const std::string_view name( _shader->name );

		if( name.find( "ambient" ) != std::string::npos )
			forwardMeshAmbient( _shader, _mesh );
	}

	inline void deferredMesh( const cShader_opengl* _shader, const cMesh_opengl* _mesh )
	{
		// DF_ProfilingScopeCpu;
		// DF_ProfilingScopeGpu;

		const cCamera* camera = cCameraManager::getInstance()->current;

		_shader->use();

		_shader->setUniformMatrix4F( "u_world_matrix", _mesh->transform->world );
		_shader->setUniformMatrix4F( "u_view_projection_matrix", camera->view_projection );

		_shader->setUniformSampler( "u_color_texture", 0 );
		_mesh->getTextures().at( aiTextureType_DIFFUSE )->bind( 0 );

		_shader->setUniformSampler( "u_normal_texture", 1 );
		_mesh->getTextures().at( aiTextureType_NORMALS )->bind( 1 );

		_shader->setUniformSampler( "u_specular_texture", 2 );
		_mesh->getTextures().at( aiTextureType_SPECULAR )->bind( 2 );

		glEnable( GL_DEPTH_TEST );

		glBindVertexArray( _mesh->vertex_array );
		glDrawElements( GL_TRIANGLES, static_cast< GLsizei >( _mesh->getIndices().size() ), GL_UNSIGNED_INT, nullptr );

		glDisable( GL_DEPTH_TEST );
	}
}
