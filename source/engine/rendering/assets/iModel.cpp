#include "iModel.h"

#include <ranges>
#include <utility>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <tracy/Tracy.hpp>

#include "iMesh.h"
#include "iTexture.h"
#include "engine/filesystem/cFileSystem.h"
#include "engine/log/Log.h"

namespace df
{
    iModel::iModel( std::string _name )
    : iRenderAsset( std::move( _name ) )
    {}

    iModel::~iModel()
    {
        ZoneScoped;

        for( const iTexture* texture : textures | std::views::values )
            delete texture;

        for( const iMesh* mesh : meshes )
            delete mesh;
    }

    void iModel::render()
    {
        ZoneScoped;

        for( iMesh* mesh : meshes )
        {
            if( !mesh->render_callback )
                mesh->render_callback = render_callback;

            mesh->render();

            if( mesh->render_callback == render_callback )
                mesh->render_callback = nullptr;
        }
    }

    bool iModel::load( std::string _folder, const unsigned _load_flags )
    {
        ZoneScoped;

        folder = std::move( _folder );

        Assimp::Importer importer;
        const aiScene*   scene = importer.ReadFile( filesystem::getPath( folder + "/model.fbx" ), _load_flags );

        if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
        {
            DF_LOG_ERROR( importer.GetErrorString() );
            return false;
        }

        return processNode( scene->mRootNode, scene );
    }
}
