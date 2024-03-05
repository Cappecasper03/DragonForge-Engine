#pragma once

#include "sRenderAsset.h"
#include "engine/rendering/assets/iMesh.h"

struct aiScene;
struct aiMesh;

namespace df::vulkan
{
    class cTexture;
    class cModel;

    class cMesh : public sRenderAsset, public iMesh
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cMesh )

        explicit cMesh( const aiMesh* _mesh, const aiScene* _scene, cModel* _parent );
        ~cMesh() override = default;

        void render() override;

    private:
        void createTextures( const aiMesh* _mesh, const aiScene* _scene ) override;
    };
}
