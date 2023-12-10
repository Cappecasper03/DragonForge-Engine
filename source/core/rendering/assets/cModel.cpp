#include "cModel.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "cMesh.h"
#include "core/memory/Memory.h"

namespace df
{
    cModel::cModel( const std::string& _folder )
    : m_folder( _folder )
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
        for( cMesh* mesh : meshes )
        {
            if( mesh )
                MEMORY_FREE( mesh );
        }
    }

    void cModel::update( const float& /*_delta_time*/ )
    {}

    void cModel::render()
    {}

    void cModel::processNode( const aiNode* _node, const aiScene* _scene )
    {
        if( !_node )
            return;

        for( unsigned i = 0; i < _node->mNumMeshes; ++i )
            meshes.push_back( MEMORY_ALLOC( cMesh, 1, _scene->mMeshes[ _node->mMeshes[ i ] ], _scene, this ) );

        for( unsigned i = 0; i < _node->mNumChildren; ++i )
            processNode( _node->mChildren[ i ], _scene );
    }
}
