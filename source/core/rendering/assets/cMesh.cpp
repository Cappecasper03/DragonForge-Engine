#include "cMesh.h"

#include <format>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glad/glad.h>

#include "cModel.h"
#include "cTexture.h"
#include "core/managers/cModelManager.h"
#include "core/managers/cRenderCallbackManager.h"

#if PROFILING
#include "core/profiling/Profiling.h"
#endif

namespace df
{
    cMesh::cMesh( const aiMesh* _mesh, const aiScene* _scene, cModel* _parent )
    : iRenderAsset( _mesh->mName.data ),
      m_parent( _parent )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        m_parent->transform.addChild( transform );
        createVertices( _mesh );
        createIndices( _mesh );
        createTextures( _mesh, _scene );
        setupRendering();
    }

    void cMesh::update( const float& _delta_time )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        iRenderAsset::update( _delta_time );
    }

    void cMesh::render()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( cModelManager::getForcedRenderCallback() )
            cRenderCallbackManager::render( cModelManager::getForcedRenderCallback(), this );
        else if( render_callback )
            cRenderCallbackManager::render( render_callback, this );
        else
            cRenderCallbackManager::render( cModelManager::getDefaultRenderCallback(), this );
    }

    void cMesh::createVertices( const aiMesh* _mesh )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        vertices.reserve( _mesh->mNumVertices );
        for( unsigned i = 0; i < _mesh->mNumVertices; ++i )
        {
            sVertex vertex{};

            if( _mesh->mVertices )
            {
                const aiVector3D& ai_vertex = _mesh->mVertices[ i ];
                vertex.position             = { ai_vertex.x, ai_vertex.y, ai_vertex.z };
            }

            if( _mesh->mNormals )
            {
                const aiVector3D& ai_normal = _mesh->mNormals[ i ];
                vertex.normal               = { ai_normal.x, ai_normal.y, ai_normal.z };
            }

            if( _mesh->mTextureCoords[ 0 ] )
            {
                const aiVector3D& ai_texture_coords = _mesh->mTextureCoords[ 0 ][ i ];
                vertex.tex_coords                   = { ai_texture_coords.x, ai_texture_coords.y };
            }

            vertices.push_back( vertex );
        }
    }

    void cMesh::createIndices( const aiMesh* _mesh )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        for( unsigned i = 0; i < _mesh->mNumFaces; ++i )
        {
            const aiFace& face = _mesh->mFaces[ i ];
            indices.reserve( indices.size() + face.mNumIndices );

            for( unsigned j = 0; j < face.mNumIndices; ++j )
                indices.push_back( face.mIndices[ j ] );
        }
    }

    void cMesh::createTextures( const aiMesh* _mesh, const aiScene* _scene )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

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

                cTexture* texture = new cTexture( texture_name, GL_TEXTURE_2D );
                if( !texture->load( full_path, 0, true, false ) )
                {
                    delete texture;
                    continue;
                }

                texture->setTextureParameterI( GL_TEXTURE_WRAP_S, GL_REPEAT );
                texture->setTextureParameterI( GL_TEXTURE_WRAP_T, GL_REPEAT );
                texture->setTextureParameterI( GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
                texture->setTextureParameterI( GL_TEXTURE_MAG_FILTER, GL_LINEAR );

                textures[ texture_name ]          = texture;
                m_parent->m_textures[ full_path ] = texture;
            }
        }
    }

    void cMesh::setupRendering() const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        glBindVertexArray( vertex_array );

        glBindBuffer( GL_ARRAY_BUFFER, m_vertex_buffer );
        glBufferData( GL_ARRAY_BUFFER, sizeof( sVertex ) * vertices.size(), &vertices.front(), GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_element_buffer );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned ) * indices.size(), &indices.front(), GL_STATIC_DRAW );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( sVertex ), nullptr );
        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( sVertex ), reinterpret_cast< void* >( offsetof( sVertex, sVertex::normal ) ) );
        glEnableVertexAttribArray( 1 );

        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( sVertex ), reinterpret_cast< void* >( offsetof( sVertex, sVertex::tex_coords ) ) );
        glEnableVertexAttribArray( 2 );

        glBindVertexArray( 0 );
    }
}
