#include "cFreeFlightCamera.h"

#include <GLFW/glfw3.h>
#include <glm/gtx/euler_angles.hpp>

#include "core/managers/cInputManager.h"

namespace df
{
    cFreeFlightCamera::cFreeFlightCamera( const float& _speed, const float& _sensitivity )
    : m_speed( _speed ),
      m_speed_multiplier( 1 ),
      m_sensitivity( _sensitivity ),
      m_rotation( .0f ),
      m_position( .0f ),
      m_movement( .0f )
    {}

    void cFreeFlightCamera::update( const float& _delta_time )
    {
        m_position += glm::vec3( transform.world[ 0 ] ) * m_movement.x * m_speed * m_speed_multiplier * _delta_time;
        m_position += glm::vec3( transform.world[ 2 ] ) * m_movement.z * m_speed * m_speed_multiplier * _delta_time;

        const glm::mat4 rot = glm::eulerAngleXYZ( glm::radians( m_rotation.x ), glm::radians( m_rotation.y ), 0.f );
        glm::mat4       trans( 1 );
        trans = translate( trans, m_position );

        transform.local = trans * rot;

        cCamera::update();
    }

    void cFreeFlightCamera::input( const input::sInput& _input )
    {
        if( _input.mouse_cursor.updated )
        {
            m_rotation.x -= static_cast< float >( _input.mouse_cursor.y_delta ) * m_sensitivity;
            m_rotation.y += static_cast< float >( _input.mouse_cursor.x_delta ) * m_sensitivity;
        }

        if( _input.mouse_scroll.updated )
        {
            m_speed_multiplier += static_cast< float >( _input.mouse_scroll.y_offset );

            if( m_speed_multiplier <= 0 )
                m_speed_multiplier = 1;
        }

        if( const input::eAction action = cInputManager::checkKey( GLFW_KEY_W ); action != input::kRepeat )
            m_movement.z -= static_cast< float >( action );

        if( const input::eAction action = cInputManager::checkKey( GLFW_KEY_S ); action != input::kRepeat )
            m_movement.z += static_cast< float >( action );

        if( const input::eAction action = cInputManager::checkKey( GLFW_KEY_A ); action != input::kRepeat )
            m_movement.x -= static_cast< float >( action );

        if( const input::eAction action = cInputManager::checkKey( GLFW_KEY_D ); action != input::kRepeat )
            m_movement.x += static_cast< float >( action );
    }
}
