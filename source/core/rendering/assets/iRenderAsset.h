#pragma once

#include "core/misc/cColor.h"
#include "core/misc/cTransform.h"
#include "core/misc/Misc.h"

namespace df
{
    class cShader;
    struct iRenderCallback;

    class iRenderAsset
    {
    public:
        DISABLE_COPY_AND_MOVE( iRenderAsset );

        iRenderAsset();
        virtual ~iRenderAsset();

        virtual void update( const float& /*_delta_time*/ ) {}
        virtual void render() = 0;

        cColor           color;
        cTransform       transform;
        unsigned         vertex_array_object;
        iRenderCallback* render_callback;

    protected:
        unsigned m_vbo;
        unsigned m_ebo;
    };
}
