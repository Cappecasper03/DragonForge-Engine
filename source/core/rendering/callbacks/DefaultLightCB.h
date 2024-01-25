#pragma once

#include "core/managers/assets/cCameraManager.h"
#include "core/rendering/cShader.h"
#include "core/rendering/assets/cLight.h"

namespace df::render_callback
{
    inline void defaultLight( const cShader* _shader, const cLight* _light )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        switch( _light->type )
        {
            case cLight::kAmbient:
                break;
            case cLight::kDirectional:
                break;
            case cLight::kPoint:
                break;
            case cLight::kSpot:
                break;
        }
    }
}
