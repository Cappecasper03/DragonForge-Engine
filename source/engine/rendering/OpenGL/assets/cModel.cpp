#include "cModel.h"

#include <ranges>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <tracy/Tracy.hpp>

#include "cMesh.h"
#include "cTexture.h"
#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"

namespace df
{
    cModel::cModel( std::string _name, std::string _folder, const unsigned _load_flags )
    : iAsset( std::move( _name ) ),
      m_folder( std::move( _folder ) )
    {
        ZoneScoped;
        
        Assimp::Importer importer;
        const aiScene*   scene = importer.ReadFile( filesystem::getPath( m_folder + "/model.fbx" ), _load_flags );

        if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
        {
            DF_LOG_ERROR( importer.GetErrorString() );
            return;
        }

        processNode( scene->mRootNode, scene );
    }

    cModel::~cModel()
    {
        ZoneScoped;
        
        for( const cTexture* texture : m_textures | std::views::values )
            delete texture;

        for( const cMesh* mesh : meshes )
            delete mesh;
    }

    void cModel::update( const float& _delta_time )
    {
        ZoneScoped;
        
        iAsset::update( _delta_time );
    }

    void cModel::render()
    {
        ZoneScoped;
        
        for( cMesh* mesh : meshes )
            mesh->render();
    }

    void cModel::processNode( const aiNode* _node, const aiScene* _scene )
    {
        ZoneScoped;
        
        if( !_node )
            return;

        for( unsigned i = 0; i < _node->mNumMeshes; ++i )
            meshes.push_back( new cMesh( _scene->mMeshes[ _node->mMeshes[ i ] ], _scene, this ) );

        for( unsigned i = 0; i < _node->mNumChildren; ++i )
            processNode( _node->mChildren[ i ], _scene );
    }
}
