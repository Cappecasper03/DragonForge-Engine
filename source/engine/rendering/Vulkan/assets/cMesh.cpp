#include "cMesh.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <filesystem>
#include <format>
#include <glad/glad.h>

#include "cModel.h"
#include "cTexture.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/vulkan/cPipeline.h"

namespace df::vulkan
{
	cMesh::cMesh( const aiMesh* _mesh, const aiScene* _scene, cModel* _parent )
		: iMesh( _mesh, _scene, _parent )
	{
		ZoneScoped;

		cMesh::createTextures( _mesh, _scene );
	}

	void cMesh::render()
	{
		ZoneScoped;

		if( cModelManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cPipeline >( cModelManager::getForcedRenderCallback(), this );
		else if( render_callback )
			cRenderCallbackManager::render< cPipeline >( render_callback, this );
		else
			cRenderCallbackManager::render< cPipeline >( cModelManager::getDefaultRenderCallback(), this );
	}

	void cMesh::createTextures( const aiMesh* _mesh, const aiScene* _scene )
	{
		ZoneScoped;

		const aiMaterial* material = _scene->mMaterials[ _mesh->mMaterialIndex ];

		for( const aiTextureType& texture_type: { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS } )
		{
			for( unsigned i = 0; i < material->GetTextureCount( texture_type ); ++i )
			{
				aiString path;
				material->GetTexture( texture_type, i, &path );

				const std::string texture_name = std::filesystem::path( path.data ).filename().replace_extension().string();
				const std::string full_path    = std::format( "{}/{}", m_parent->folder, path.data );

				if( auto it = m_parent->textures.find( full_path ); it != m_parent->textures.end() && it->second )
				{
					textures[ texture_name ] = it->second;
					continue;
				}

				cTexture* texture = new cTexture( texture_name, GL_TEXTURE_2D );
				if( !texture->load( full_path, false, true, false ) )
				{
					delete texture;
					continue;
				}

				texture->setTextureParameterI( GL_TEXTURE_WRAP_S, GL_REPEAT );
				texture->setTextureParameterI( GL_TEXTURE_WRAP_T, GL_REPEAT );
				texture->setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
				texture->setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );

				textures[ texture_name ]        = texture;
				m_parent->textures[ full_path ] = texture;
			}
		}
	}
}
