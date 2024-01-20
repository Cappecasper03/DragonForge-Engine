#include "cModel.h"

#include <ranges>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "cMesh.h"
#include "cTexture.h"
#include "core/filesystem/cFileSystem.h"
#include "core/log/Log.h"

#if PROFILING
#include "core/profiling/Profiling.h"
#endif

namespace df
{
    cModel::cModel( std::string _name, std::string _folder, const unsigned _load_flags )
    : iAsset( std::move( _name ) ),
      m_folder( std::move( _folder ) )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        Assimp::Importer importer;
        const aiScene*   scene = importer.ReadFile( filesystem::getPath( m_folder + "/model.fbx" ), _load_flags );

        if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
        {
            LOG_ERROR( importer.GetErrorString() );
            return;
        }

        processNode( scene->mRootNode, scene );
    }

    cModel::~cModel()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        for( const cTexture* texture : m_textures | std::views::values )
            delete texture;

        for( const cMesh* mesh : meshes )
            delete mesh;
    }

    void cModel::update( const float& _delta_time )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        iAsset::update( _delta_time );
    }

    void cModel::render()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        for( cMesh* mesh : meshes )
            mesh->render();
    }

    void cModel::processNode( const aiNode* _node, const aiScene* _scene )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( !_node )
            return;

        for( unsigned i = 0; i < _node->mNumMeshes; ++i )
            meshes.push_back( new cMesh( _scene->mMeshes[ _node->mMeshes[ i ] ], _scene, this ) );

        for( unsigned i = 0; i < _node->mNumChildren; ++i )
            processNode( _node->mChildren[ i ], _scene );
    }
}
