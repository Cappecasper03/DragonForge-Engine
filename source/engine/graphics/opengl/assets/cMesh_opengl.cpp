#include "cMesh_opengl.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <filesystem>

#include "cModel_opengl.h"
#include "cTexture2D_opengl.h"
#include "engine/graphics/assets/iMesh.h"
#include "engine/graphics/opengl/cShader_opengl.h"
#include "engine/graphics/opengl/OpenGlTypes.h"
#include "engine/graphics/types/sTextureParameter.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cMesh_opengl::cMesh_opengl( const aiMesh* _mesh, const aiScene* _scene, cModel_opengl* _parent )
		: iMesh( _mesh, _scene, _parent )
		, m_push_constant( cBuffer_opengl::kUniform )
	{
		DF_ProfilingScopeCpu;

		cMesh_opengl::createTextures( _mesh, _scene );

		m_vertex_array.bind();

		m_vertex_buffer.bind();
		m_vertex_buffer.setData( sizeof( sVertex ) * m_vertices.size(), m_vertices.data(), cBuffer_opengl::kStaticDraw );

		m_index_buffer.bind();
		m_index_buffer.setData( sizeof( unsigned ) * m_indices.size(), m_indices.data(), cBuffer_opengl::kStaticDraw );

		m_vertex_array.setAttribute( 0, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::position ) );
		m_vertex_array.setAttribute( 1, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::normal ) );
		m_vertex_array.setAttribute( 2, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::tangent ) );
		m_vertex_array.setAttribute( 3, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::bitangent ) );
		m_vertex_array.setAttribute( 4, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::tex_coords ) );
		m_vertex_array.unbind();

		m_push_constant.bind();
		m_push_constant.setData( sizeof( sPushConstants ), nullptr, cBuffer_opengl::kDynamicDraw );
		m_push_constant.unbind();
	}

	void cMesh_opengl::render()
	{
		DF_ProfilingScopeCpu;

		if( cModelManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cShader_opengl >( cModelManager::getForcedRenderCallback(), this );
		else if( m_render_callback )
			cRenderCallbackManager::render< cShader_opengl >( m_render_callback, this );
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

				std::filesystem::path file_path    = m_parent->m_path;
				std::filesystem::path filename     = std::filesystem::path( path.data ).filename();
				const std::string     full_path    = fmt::format( "{}/{}", file_path.parent_path().string(), filename.string() );
				const std::string     texture_name = filename.replace_extension().string();

				if( auto it = m_parent->m_textures.find( full_path ); it != m_parent->m_textures.end() && it->second )
				{
					m_textures[ texture_type ] = it->second;
					continue;
				}

				const cTexture2D::sImageInfo   image_info = cTexture2D::getInfoFromFile( full_path );
				const cTexture2D::sDescription description{
					.name       = texture_name,
					.size       = image_info.size,
					.mip_levels = 1,
					.format     = image_info.format,
				};
				cTexture2D_opengl* texture = reinterpret_cast< cTexture2D_opengl* >( cTexture2D::create( description ) );
				if( !texture->uploadDataFromFile( full_path, image_info.format ) )
				{
					delete texture;
					continue;
				}

				texture->setInteger( sTextureParameter::kWrapS, sTextureParameter::kRepeat );
				texture->setInteger( sTextureParameter::kWrapT, sTextureParameter::kRepeat );
				texture->setInteger( sTextureParameter::kMinFilter, sTextureParameter::kLinearMipmapLinear );
				texture->setInteger( sTextureParameter::kMagFilter, sTextureParameter::kLinear );

				m_textures[ texture_type ]        = texture;
				m_parent->m_textures[ full_path ] = texture;
			}

			if( m_textures.contains( texture_type ) )
				continue;

			if( auto it = m_parent->m_textures.find( "df_white" ); it != m_parent->m_textures.end() && it->second )
			{
				m_textures[ texture_type ] = it->second;
				continue;
			}

			const cTexture2D::sDescription description{
				.name       = "df_white",
				.size       = cVector2u( 1 ),
				.mip_levels = 1,
				.format     = sTextureFormat::kRed,
			};
			cTexture2D_opengl* texture         = reinterpret_cast< cTexture2D_opengl* >( cTexture2D::create( description ) );
			m_textures[ texture_type ]         = texture;
			m_parent->m_textures[ "df_white" ] = texture;
		}
	}
}
