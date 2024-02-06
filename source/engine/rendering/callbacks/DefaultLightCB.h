#pragma once

#include "engine/managers/assets/cCameraManager.h"
#include "engine/rendering/cShader.h"
#include "engine/rendering/assets/cLight.h"

namespace df::render_callback
{
    inline void defaultLight( const cShader* _shader, const cLight* _light )
    {
#ifdef PROFILING
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
