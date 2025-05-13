#include "cDefaultMesh_opengl.h"

#include <assimp/material.h>
#include <glad/glad.h>

#include "engine/core/utils/cTransform.h"
#include "engine/graphics/assets/iTexture.h"
#include "engine/graphics/cameras/cCamera.h"
#include "engine/managers/cCameraManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/profiling/ProfilingMacros_opengl.h"

namespace df::opengl::render_callbacks
{
	void cDefaultMesh_opengl::forwardMesh( const cShader_opengl* _shader, const cMesh_opengl* _mesh )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		_shader->use();

		const iMesh::sPushConstants push_constants{
			.world_matrix    = _mesh->m_transform->m_world,
			.camera_position = cVector3f( cCameraManager::getInstance()->m_current->m_transform->m_world.position() ),
		};

		_mesh->m_push_constant.bind();
		_mesh->m_push_constant.setSubData( 0, sizeof( iMesh::sPushConstants ), &push_constants );
		_mesh->m_push_constant.unbind();
		_mesh->m_push_constant.bindBase( 0 );

		_mesh->getTextures().at( aiTextureType_DIFFUSE )->bind( 0 );
		_mesh->getTextures().at( aiTextureType_NORMALS )->bind( 1 );

		glEnable( kDepthTest );
		glEnable( kBlend );

		glBlendFunc( kSrcAlpha, kOneMinusSrcAlpha );

		_mesh->m_vertex_array.bind();
		glDrawElements( kTriangles, static_cast< GLsizei >( _mesh->getIndices().size() ), kUnsignedInt, nullptr );

		glDisable( kBlend );
		glDisable( kDepthTest );
	}

	void cDefaultMesh_opengl::deferredMesh( const cShader_opengl* _shader, const cMesh_opengl* _mesh )
	{
		DF_ProfilingScopeCpu;
		DF_ProfilingScopeGpu;

		_shader->use();

		const iMesh::sPushConstants push_constants{
			.world_matrix    = _mesh->m_transform->m_world,
			.camera_position = cVector3f( cCameraManager::getInstance()->m_current->m_transform->m_world.position() ),
		};

		_mesh->m_push_constant.bind();
		_mesh->m_push_constant.setSubData( 0, sizeof( iMesh::sPushConstants ), &push_constants );
		_mesh->m_push_constant.unbind();
		_mesh->m_push_constant.bindBase( 0 );

		_mesh->getTextures().at( aiTextureType_DIFFUSE )->bind( 0 );
		_mesh->getTextures().at( aiTextureType_NORMALS )->bind( 1 );

		glEnable( kDepthTest );

		_mesh->m_vertex_array.bind();
		glDrawElements( kTriangles, static_cast< GLsizei >( _mesh->getIndices().size() ), kUnsignedInt, nullptr );

		glDisable( kDepthTest );
	}
}