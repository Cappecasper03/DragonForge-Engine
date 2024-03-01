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
        : name( std::move( _name ) ),
          transform( nullptr )
        {}

        virtual      ~iAsset() = default;
        virtual void update( const float& /*_delta_time*/ = 0 ) { if( transform ) transform->update(); }
        virtual void render() {}

        const std::string name;
        cTransform*       transform;
    };

    /*namespace vulkan
    {
        struct sRendererSpecific : iRendererSpecific
        {
            DF_DISABLE_COPY_AND_MOVE( sRendererSpecific );

            sRendererSpecific();
            ~sRendererSpecific();

            VkBuffer       vertex_buffer;
            VkDeviceMemory vertex_buffer_memory;
        };
    }*/

    class iRenderAsset : public iAsset
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( iRenderAsset );

        explicit iRenderAsset( std::string _name );
        ~iRenderAsset() override;

        void render() override = 0;

        iRenderCallback* render_callback;
    };
}
