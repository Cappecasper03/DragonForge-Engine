#pragma once

#include <string>

#include "iRenderAsset.h"
#include "core/misc/Misc.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace df
{
    class cMesh;

    class cModel : public iAsset
    {
    public:
        DISABLE_COPY_AND_MOVE( cModel )

        friend cMesh;

        cModel( const std::string& _folder );
        ~cModel() override;

        void update( const float& _delta_time ) override;
        void render() override;

        std::vector< cMesh* > meshes;

    private:
        void   processNode( const aiNode* _node, const aiScene* _scene );
        cMesh* processMesh( aiMesh* _mesh, const aiScene* _scene );

        const std::string m_folder;
    };
}
