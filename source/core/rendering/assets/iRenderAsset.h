#pragma once

#include <glm/ext/matrix_float4x4.hpp>

#include "core/misc/Misc.h"

namespace df
{
    class iRenderAsset
    {
    public:
        DISABLE_COPY_AND_MOVE( iRenderAsset );

        iRenderAsset();
        virtual ~iRenderAsset();

        virtual void render() = 0;

        glm::mat4 matrix;

    protected:
        unsigned m_vao;
        unsigned m_vbo;
        unsigned m_ebo;
    };
}
