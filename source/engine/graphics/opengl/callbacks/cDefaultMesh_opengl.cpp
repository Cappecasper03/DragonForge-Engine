#include "cDefaultMesh_opengl.h"

#include <assimp/material.h>
#include <glad/glad.h>

#include "engine/graphics/cameras/cCamera.h"
#include "engine/managers/cCameraManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_opengl.h"
#include "engine/graphics/assets/iTexture.h"
#include "engine/core/utils/cTransform.h"

namespace df::opengl::render_callbacks
{
	void cDefaultMesh_opengl::forwardMeshAmbient( const cShader_opengl* _shader, const cMesh_opengl* _mesh )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		const cCamera* camera = cCameraManager::getInstance()->current;

		_shader->use();

		_shader->setUniformMatrix4F( "u_world_matrix", _mesh->transform->world );
		_shader->setUniformMatrix4F( "u_view_projection_matrix", camera->view_projection );

		_shader->setUniformSampler( "u_color_texture", 0 );
		_mesh->getTextures().at( aiTextureType_DIFFUSE )->bind();

		glEnable( kDepthTest );
		glEnable( kBlend );

		glBlendFunc( kSrcAlpha, kOneMinusSrcAlpha );

		_mesh->vertex_array.bind();
		glDrawElements( kTriangles, static_cast< GLsizei >( _mesh->getIndices().size() ), kUnsignedInt, nullptr );

		glDisable( kBlend );
		glDisable( kDepthTest );
	}

	void cDefaultMesh_opengl::forwardMesh( const cShader_opengl* _shader, const cMesh_opengl* _mesh )
	{
		DF_ProfilingScopeCpu;

		const std::string_view name( _shader->name );

		if( name.find( "ambient" ) != std::string::npos )
			forwardMeshAmbient( _shader, _mesh );
	}

	void cDefaultMesh_opengl::deferredMesh( const cShader_opengl* _shader, const cMesh_opengl* _mesh )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

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

		glEnable( kDepthTest );

		_mesh->vertex_array.bind();
		glDrawElements( kTriangles, static_cast< GLsizei >( _mesh->getIndices().size() ), kUnsignedInt, nullptr );

		glDisable( kDepthTest );
	}
}