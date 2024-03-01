#pragma once

#include "engine/misc/Misc.h"
#include "engine/rendering/assets/iRenderAsset.h"

namespace df::opengl
{
    struct sRenderAsset : iRenderAsset
    {
        DF_DISABLE_COPY_AND_MOVE( sRenderAsset );

        explicit sRenderAsset( std::string _name );
        ~sRenderAsset() override;

        unsigned vertex_array;
        unsigned vertex_buffer;
        unsigned element_buffer;
    };
}
