#include "cMesh.h"

#include <filesystem>
#include <format>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glad/glad.h>

#include "cModel.h"
#include "cTexture.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/managers/assets/cModelManager.h"
#include "engine/rendering/cRendererSingleton.h"
#include "engine/rendering/OpenGL/callbacks/DefaultFontCB.h"

namespace df
{
    cMesh::cMesh( const aiMesh* _mesh, const aiScene* _scene, cModel* _parent )
    : iRenderAsset( _mesh->mName.data ),
      m_parent( _parent )
    {
        ZoneScoped;

        m_parent->transform.addChild( transform );
        createVertices( _mesh );
        createIndices( _mesh );
        createTextures( _mesh, _scene );

        switch( cRendererSingleton::getRenderInstanceType() )
        {
            case cRendererSingleton::kOpenGL: { setupRenderingOpenGL(); }
            break;
            case cRendererSingleton::kVulkan:
            {}
            break;
        }
    }

    void cMesh::update( const float& _delta_time )
    {
        ZoneScoped;

        iRenderAsset::update( _delta_time );
    }

    void cMesh::render()
    {
        ZoneScoped;

        switch( cRendererSingleton::getRenderInstanceType() )
        {
            case cRendererSingleton::kOpenGL:
            {
                if( cModelManager::getForcedRenderCallback() )
                    cRenderCallbackManager::render< opengl::cShader >( cModelManager::getForcedRenderCallback(), this );
                else if( render_callback )
                    cRenderCallbackManager::render< opengl::cShader >( render_callback, this );
                else
                    cRenderCallbackManager::render< opengl::cShader >( cModelManager::getDefaultRenderCallback(), this );
            }
            break;
            case cRendererSingleton::kVulkan:
            {}
            break;
        }
    }

    void cMesh::createVertices( const aiMesh* _mesh )
    {
        ZoneScoped;

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

            if( _mesh->mTangents )
            {
                const aiVector3D& ai_tangent = _mesh->mTangents[ i ];
                vertex.tangent               = { ai_tangent.x, ai_tangent.y, ai_tangent.z };
            }

            if( _mesh->mBitangents )
            {
                const aiVector3D& ai_bitangent = _mesh->mBitangents[ i ];
                vertex.bitangent               = { ai_bitangent.x, ai_bitangent.y, ai_bitangent.z };
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
        ZoneScoped;

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
        ZoneScoped;

        const aiMaterial* material = _scene->mMaterials[ _mesh->mMaterialIndex ];

        const std::vector texture_types = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_NORMALS };

        for( const aiTextureType& texture_type : texture_types )
        {
            for( unsigned i = 0; i < material->GetTextureCount( texture_type ); ++i )
            {
                aiString path;
                material->GetTexture( texture_type, i, &path );

                const std::string texture_name = std::filesystem::path( path.data ).filename().replace_extension().string();
                const std::string full_path    = std::format( "{}/{}", m_parent->m_folder, path.data );

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

    void cMesh::setupRenderingOpenGL() const
    {
        ZoneScoped;

        const opengl::sRendererSpecific* specific = reinterpret_cast< opengl::sRendererSpecific* >( render_specific );

        glBindVertexArray( specific->vertex_array );

        glBindBuffer( GL_ARRAY_BUFFER, specific->vertex_buffer );
        glBufferData( GL_ARRAY_BUFFER, sizeof( sVertex ) * vertices.size(), &vertices.front(), GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, specific->element_buffer );
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
}
