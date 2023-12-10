#include "cMesh.h"

#include <format>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glad/glad.h>

#include "cModel.h"
#include "cTexture.h"
#include "core/memory/Memory.h"

namespace df
{
    cMesh::cMesh( const aiMesh* _mesh, const aiScene* _scene, cModel* _parent )
    : name( _mesh->mName.data ),
      m_parent( _parent )
    {
        createVertices( _mesh );
        createIndices( _mesh );
        createTextures( _mesh, _scene );
    }

    void cMesh::update( const float& /*_delta_time*/ )
    {}

    void cMesh::render() {}

    void cMesh::createVertices( const aiMesh* _mesh )
    {
        vertices.reserve( _mesh->mNumVertices );
        for( unsigned i = 0; i < _mesh->mNumVertices; ++i )
        {
            sVertex vertex{};

            if( _mesh->mVertices )
            {
                const aiVector3D& ai_vertex = _mesh->mVertices[ i ];
                vertex.position.x           = ai_vertex.x;
                vertex.position.y           = ai_vertex.y;
                vertex.position.z           = ai_vertex.z;
            }

            if( _mesh->mNormals )
            {
                const aiVector3D& ai_normal = _mesh->mNormals[ i ];
                vertex.normal.x             = ai_normal.x;
                vertex.normal.y             = ai_normal.y;
                vertex.normal.z             = ai_normal.z;
            }

            if( *_mesh->mTextureCoords )
            {
                const aiVector3D* ai_texture_coords = *_mesh->mTextureCoords;
                vertex.tex_coords.x                 = ai_texture_coords[ i ].x;
                vertex.tex_coords.y                 = ai_texture_coords[ i ].y;
            }

            vertices.push_back( vertex );
        }
    }

    void cMesh::createIndices( const aiMesh* _mesh )
    {
        for( unsigned i = 0; i < _mesh->mNumFaces; ++i )
        {
            const aiFace& face = _mesh->mFaces[ i ];

            for( unsigned j = 0; j < face.mNumIndices; ++j )
                indices.push_back( face.mIndices[ j ] );
        }
    }

    void cMesh::createTextures( const aiMesh* _mesh, const aiScene* _scene )
    {
        const aiMaterial* material = _scene->mMaterials[ _mesh->mMaterialIndex ];

        const std::vector texture_types = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS };

        for( const aiTextureType& texture_type : texture_types )
        {
            for( unsigned i = 0; i < material->GetTextureCount( texture_type ); ++i )
            {
                aiString path;
                material->GetTexture( texture_type, i, &path );

                std::string texture_name = path.data;
                texture_name             = texture_name.substr( texture_name.find_last_of( '\\' ) + 1 );
                texture_name             = texture_name.substr( 0, texture_name.find_last_of( '.' ) );

                const std::string full_path = std::format( "{}/{}", m_parent->m_folder, path.data );
                if( auto it = m_parent->m_textures.find( full_path ); it != m_parent->m_textures.end() && it->second )
                {
                    textures[ texture_name ] = it->second;
                    continue;
                }

                cTexture* texture = MEMORY_ALLOC( cTexture, 1, GL_TEXTURE_2D );
                if( !texture->load( full_path ) )
                {
                    MEMORY_FREE( texture );
                    continue;
                }

                textures[ texture_name ]          = texture;
                m_parent->m_textures[ full_path ] = texture;
            }
        }
    }
}
