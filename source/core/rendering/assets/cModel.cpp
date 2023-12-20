#include "cModel.h"

#include <ranges>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "cMesh.h"
#include "cTexture.h"
#include "core/log/Log.h"

namespace df
{
    cModel::cModel( std::string _name, std::string _folder )
    : iAsset( std::move( _name ) ),
      m_folder( std::move( _folder ) )
    {
        Assimp::Importer importer;
        const aiScene*   scene = importer.ReadFile( m_folder + "/model.fbx", aiProcess_Triangulate | aiProcess_FlipUVs );

        if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
        {
            LOG_ERROR( importer.GetErrorString() );
            return;
        }

        processNode( scene->mRootNode, scene );
    }

    cModel::~cModel()
    {
        for( const cTexture* texture : m_textures | std::views::values )
            delete texture;

        for( const cMesh* mesh : meshes )
            delete mesh;
    }

    void cModel::update( const float& /*_delta_time*/ )
    {
        transform.update();
    }

    void cModel::render()
    {
        for( cMesh* mesh : meshes )
            mesh->render();
    }

    void cModel::processNode( const aiNode* _node, const aiScene* _scene )
    {
        if( !_node )
            return;

        for( unsigned i = 0; i < _node->mNumMeshes; ++i )
            meshes.push_back( new cMesh( _scene->mMeshes[ _node->mMeshes[ i ] ], _scene, this ) );

        for( unsigned i = 0; i < _node->mNumChildren; ++i )
            processNode( _node->mChildren[ i ], _scene );
    }
}
