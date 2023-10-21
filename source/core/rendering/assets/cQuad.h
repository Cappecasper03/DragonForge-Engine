#pragma once

#include <glm/vec3.hpp>

#include "iRenderAsset.h"

namespace df
{
    class cQuad : public iRenderAsset
    {
    public:
        DISABLE_COPY_AND_MOVE( cQuad );

        struct sVertex
        {
            glm::vec3 position;
            glm::vec2 tex_coord;
        };

        cQuad( const float& _width, const float& _height );
        ~cQuad() override = default;

        void render() override;

    protected:
        sVertex  m_vertices[ 4 ];
        unsigned m_indices[ 6 ];
    };
}
