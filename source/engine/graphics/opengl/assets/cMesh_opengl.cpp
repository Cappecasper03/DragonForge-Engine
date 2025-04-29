#include "cMesh_opengl.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <filesystem>
#include <glad/glad.h>

#include "cModel_opengl.h"
#include "cTexture_opengl.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/graphics/opengl/cShader_opengl.h"
#include "graphics/opengl/OpenGlTypes.h"

namespace df::opengl
{
	cMesh_opengl::cMesh_opengl( const aiMesh* _mesh, const aiScene* _scene, cModel_opengl* _parent )
		: iMesh( _mesh, _scene, _parent )
	{
		DF_ProfilingScopeCpu;

		cMesh_opengl::createTextures( _mesh, _scene );

		vertex_array.bind();

		vertex_buffer.bind();
		vertex_buffer.setData( sizeof( sVertex ) * m_vertices.size(), m_vertices.data(), cBuffer_opengl::kStaticDraw );

		index_buffer.bind();
		index_buffer.setData( sizeof( unsigned ) * m_indices.size(), m_indices.data(), cBuffer_opengl::kStaticDraw );

		vertex_array.setAttribute( 0, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::position ) );
		vertex_array.setAttribute( 1, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::normal ) );
		vertex_array.setAttribute( 2, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::tangent ) );
		vertex_array.setAttribute( 3, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::bitangent ) );
		vertex_array.setAttribute( 4, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::tex_coords ) );
		vertex_array.unbind();
	}

	void cMesh_opengl::render()
	{
		DF_ProfilingScopeCpu;

		if( cModelManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cShader_opengl >( cModelManager::getForcedRenderCallback(), this );
		else if( render_callback )
			cRenderCallbackManager::render< cShader_opengl >( render_callback, this );
		else
			cRenderCallbackManager::render< cShader_opengl >( cModelManager::getDefaultRenderCallback(), this );
	}

	void cMesh_opengl::createTextures( const aiMesh* _mesh, const aiScene* _scene )
	{
		DF_ProfilingScopeCpu;

		const aiMaterial* material = _scene->mMaterials[ _mesh->mMaterialIndex ];

		for( const aiTextureType& texture_type: { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS } )
		{
			for( unsigned i = 0; i < material->GetTextureCount( texture_type ); ++i )
			{
				aiString path;
				material->GetTexture( texture_type, i, &path );

				const std::string texture_name = std::filesystem::path( path.data ).filename().replace_extension().string();
				const std::string full_path    = fmt::format( "{}/{}", m_parent->folder, path.data );

				if( auto it = m_parent->textures.find( full_path ); it != m_parent->textures.end() && it->second )
				{
					m_textures[ texture_type ] = it->second;
					continue;
				}

				cTexture_opengl* texture = new cTexture_opengl( texture_name, cTexture_opengl::k2D );
				if( !texture->load( full_path, true ) )
				{
					delete texture;
					continue;
				}

				texture->setTextureParameterI( cTexture_opengl::kTextureWrapS, cTexture_opengl::kRepeat );
				texture->setTextureParameterI( cTexture_opengl::kTextureWrapT, cTexture_opengl::kRepeat );
				texture->setTextureParameterI( cTexture_opengl::kTextureMinFilter, cTexture_opengl::kLinearMipmapLinear );
				texture->setTextureParameterI( cTexture_opengl::kTextureMagFilter, cTexture_opengl::kLinear );

				m_textures[ texture_type ]      = texture;
				m_parent->textures[ full_path ] = texture;
			}

			if( m_textures.contains( texture_type ) )
				continue;

			if( auto it = m_parent->textures.find( "white" ); it != m_parent->textures.end() && it->second )
			{
				m_textures[ texture_type ] = it->second;
				continue;
			}

			cTexture_opengl* texture      = new cTexture_opengl( "white", cTexture_opengl::k2D );
			m_textures[ texture_type ]    = texture;
			m_parent->textures[ "white" ] = texture;
		}
	}
}
