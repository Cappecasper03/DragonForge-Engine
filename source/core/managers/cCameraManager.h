#pragma once

#include "core/misc/cColor.h"
#include "core/misc/iSingleton.h"
#include "core/rendering/camera/cCamera.h"

namespace df
{
    class cCameraManager final : public iSingleton< cCameraManager >
    {
    public:
        DISABLE_COPY_AND_MOVE( cCameraManager );

        cCameraManager();
        ~cCameraManager() override;

        cCamera* create( const std::string& _name, const cCamera::eType& _type, const cColor& _clear_color, const float& _fov, const float& _near_clip = .1f, const float& _far_clip = 100 );
        bool     add( const std::string& _name, cCamera* _camera );

        bool destroy( const std::string& _name );
        bool destroy( const cCamera* _camera );

        cCamera* get( const std::string& _name );

        cCamera* current;

    private:
        std::map< std::string, cCamera* > m_cameras;
    };
}
