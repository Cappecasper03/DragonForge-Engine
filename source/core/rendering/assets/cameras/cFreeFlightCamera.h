#pragma once

#include "cCamera.h"
#include "core/input/iPlayerController.h"

namespace df
{
    class cFreeFlightCamera : public cCamera, public iPlayerController
    {
    public:
        explicit cFreeFlightCamera( const float& _speed, const float& _sensitivity );

        void update( const float& _delta_time ) override;
        void input( const input::sInput& _input ) override;

    protected:
        float m_speed;
        float m_speed_multiplier;
        float m_sensitivity;

        glm::vec3 m_rotation;
        glm::vec3 m_position;
        glm::vec3 m_movement;
    };
}
