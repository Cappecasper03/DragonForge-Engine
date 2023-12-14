#pragma once

#include <string>

#include "core/misc/cColor.h"
#include "core/misc/cTransform.h"
#include "core/misc/Misc.h"

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
        virtual void update( const float& /*_delta_time*/ ) {}
        virtual void render() {}

        const std::string name;
    };

    class iRenderAsset : public iAsset
    {
    public:
        DISABLE_COPY_AND_MOVE( iRenderAsset );

        explicit iRenderAsset( std::string _name );
        ~iRenderAsset() override;

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
