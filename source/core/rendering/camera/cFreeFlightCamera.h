#pragma once

#include "core/input/iPlayerController.h"
#include "core/rendering/camera/cCamera.h"

namespace df
{
    class cFreeFlightCamera : public cCamera, public iPlayerController
    {
    public:
        explicit cFreeFlightCamera( const float& _speed );

        void update( const float& _delta_time ) override;
        void input( const input::sInput& _input ) override;

    protected:
        float     m_speed;
        glm::vec3 m_rotation;
        glm::vec3 m_position;
    };
}
