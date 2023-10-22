﻿#pragma once

#include <glm/vec3.hpp>

#include "iRenderAsset.h"
#include "core/misc/cColor.h"

namespace df
{
    class cTexture;
    
    class cQuad : public iRenderAsset
    {
    public:
        DISABLE_COPY_AND_MOVE( cQuad );

        struct sVertex
        {
            glm::vec3 position;
            glm::vec2 tex_coord;
        };

        cQuad( const float& _width, const float& _height, const cColor& _color );
        ~cQuad() override = default;

        void render() override;

        cColor    color;
        cTexture* texture;

    protected:
        sVertex  m_vertices[ 4 ];
        unsigned m_indices[ 6 ];
    };
}
