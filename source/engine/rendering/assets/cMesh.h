#pragma once

#include <unordered_map>

#include "iRenderAsset.h"

struct aiScene;
struct aiMesh;

namespace df
{
    class cTexture;
    class cModel;

    class cMesh : public iRenderAsset
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cMesh )

        struct sVertex
        {
            glm::vec3 position   = glm::vec3( 0 );
            glm::vec3 normal     = glm::vec3( 0 );
            glm::vec3 tangent    = glm::vec3( 0 );
            glm::vec3 bitangent  = glm::vec3( 0 );
            glm::vec2 tex_coords = glm::vec2( 0 );
        };

        explicit cMesh( const aiMesh* _mesh, const aiScene* _scene, cModel* _parent );
        ~cMesh() override = default;

        void update( const float& _delta_time ) override;
        void render() override;

        std::vector< sVertex >                       vertices;
        std::vector< unsigned >                      indices;
        std::unordered_map< std::string, cTexture* > textures;

    private:
        void createVertices( const aiMesh* _mesh );
        void createIndices( const aiMesh* _mesh );
        void createTextures( const aiMesh* _mesh, const aiScene* _scene );
        void setupRendering() const;

        cModel* m_parent;
    };
}
