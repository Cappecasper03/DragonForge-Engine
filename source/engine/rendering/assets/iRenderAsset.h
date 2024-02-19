#pragma once

#include <string>
#include <vulkan/vulkan_core.h>

#include "engine/misc/cTransform.h"
#include "engine/misc/Misc.h"

namespace df
{
    struct iRenderCallback;

    struct iAsset
    {
        DF_DISABLE_COPY_AND_MOVE( iAsset );

        explicit iAsset( std::string _name )
        : name( std::move( _name ) )
        {}

        virtual      ~iAsset() = default;
        virtual void update( const float& /*_delta_time*/ = 0 ) { transform.update(); }
        virtual void render() {}

        const std::string name;
        cTransform        transform;
    };

    struct iRendererSpecific
    {};

    namespace opengl
    {
        struct sRendererSpecific : iRendererSpecific
        {
            DF_DISABLE_COPY_AND_MOVE( sRendererSpecific );

            sRendererSpecific();
            ~sRendererSpecific();

            unsigned vertex_array;
            unsigned vertex_buffer;
            unsigned element_buffer;
        };
    }

    namespace vulkan
    {
        struct sRendererSpecific : iRendererSpecific
        {
            DF_DISABLE_COPY_AND_MOVE( sRendererSpecific );

            sRendererSpecific();
            ~sRendererSpecific();

            VkBuffer vertex_buffer;
        };
    }

    class iRenderAsset : public iAsset
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( iRenderAsset );

        explicit iRenderAsset( std::string _name );
        ~iRenderAsset() override;

        void update( const float& _delta_time ) override { iAsset::update( _delta_time ); }
        void render() override = 0;

        iRendererSpecific* render_specific;
        iRenderCallback*   render_callback;
    };
}
