#pragma once

#include <string>

#include "engine/misc/cTransform.h"
#include "engine/misc/Misc.h"

namespace df
{
    class cShader;
    struct iRenderCallback;

    struct iAsset
    {
        DISABLE_COPY_AND_MOVE( iAsset );

        explicit iAsset( std::string _name )
        : name( std::move( _name ) )
        {}

        virtual      ~iAsset() = default;
        virtual void update( const float& /*_delta_time*/ = 0 ) { transform.update(); }
        virtual void render() {}

        const std::string name;
        cTransform        transform;
    };

    class iRenderAsset : public iAsset
    {
    public:
        DISABLE_COPY_AND_MOVE( iRenderAsset );

        explicit iRenderAsset( std::string _name );
        ~iRenderAsset() override;

        void update( const float& _delta_time ) override { iAsset::update( _delta_time ); }
        void render() override = 0;

        unsigned         vertex_array;
        iRenderCallback* render_callback;

    protected:
        unsigned m_vertex_buffer;
        unsigned m_element_buffer;
    };
}
