#include "cMesh_opengl.h"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <filesystem>
#include <format>
#include <glad/glad.h>

#include "cModel_opengl.h"
#include "cTexture_opengl.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/OpenGL/cShader_opengl.h"

namespace df::opengl
{
	cMesh_opengl::cMesh_opengl( const aiMesh* _mesh, const aiScene* _scene, cModel_opengl* _parent )
		: iMesh( _mesh, _scene, _parent )
	{
		ZoneScoped;

		cMesh_opengl::createTextures( _mesh, _scene );

		glBindVertexArray( vertex_array );

		glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof( sVertex ) * vertices.size(), &vertices.front(), GL_STATIC_DRAW );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, element_buffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned ) * indices.size(), &indices.front(), GL_STATIC_DRAW );

		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( sVertex ), nullptr );
		glEnableVertexAttribArray( 0 );

		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( sVertex ), reinterpret_cast< void* >( offsetof( sVertex, sVertex::normal ) ) );
		glEnableVertexAttribArray( 1 );

		glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof( sVertex ), reinterpret_cast< void* >( offsetof( sVertex, sVertex::tangent ) ) );
		glEnableVertexAttribArray( 2 );

		glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( sVertex ), reinterpret_cast< void* >( offsetof( sVertex, sVertex::bitangent ) ) );
		glEnableVertexAttribArray( 3 );

		glVertexAttribPointer( 4, 2, GL_FLOAT, GL_FALSE, sizeof( sVertex ), reinterpret_cast< void* >( offsetof( sVertex, sVertex::tex_coords ) ) );
		glEnableVertexAttribArray( 4 );

		glBindVertexArray( 0 );
	}

	void cMesh_opengl::render()
	{
		ZoneScoped;

		if( cModelManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cShader_opengl >( cModelManager::getForcedRenderCallback(), this );
		else if( render_callback )
			cRenderCallbackManager::render< cShader_opengl >( render_callback, this );
		else
			cRenderCallbackManager::render< cShader_opengl >( cModelManager::getDefaultRenderCallback(), this );
	}

	void cMesh_opengl::createTextures( const aiMesh* _mesh, const aiScene* _scene )
	{
		ZoneScoped;

		const aiMaterial* material = _scene->mMaterials[ _mesh->mMaterialIndex ];

		const std::vector texture_types = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS };

		for( const aiTextureType& texture_type: texture_types )
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

				cTexture_opengl* texture = new cTexture_opengl( texture_name, GL_TEXTURE_2D );
				if( !texture->load( full_path, 0, true, false ) )
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
