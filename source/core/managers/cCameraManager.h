#pragma once

#include <unordered_map>

#include "core/misc/cColor.h"
#include "core/misc/iSingleton.h"
#include "core/rendering/cameras/cCamera.h"

namespace df
{
    class cCameraManager final : public iSingleton< cCameraManager >
    {
    public:
        DISABLE_COPY_AND_MOVE( cCameraManager );

        cCameraManager();
        ~cCameraManager() override { clear(); }

        static cCamera* create( const std::string& _name, const cCamera::eType& _type, const cColor& _clear_color, const float& _fov, const float& _near_clip = .1f, const float& _far_clip = 100 );

        static bool destroy( const std::string& _name );
        static bool destroy( const cCamera* _camera );
        static void clear();

        static cCamera* get( const std::string& _name );

        cCamera* current;

    private:
        std::unordered_map< std::string, cCamera* > m_cameras;
    };
}
