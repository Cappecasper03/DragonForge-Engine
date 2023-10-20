#include "cFreeFlightCamera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace df
{
    cFreeFlightCamera::cFreeFlightCamera( const float& _speed )
    : m_speed( _speed ),
      m_rotation( .0f ),
      m_position( .0f )
    {}

    void cFreeFlightCamera::update( const float& _delta_time )
    {
        glm::mat4 rot = glm::eulerAngleXY( m_rotation.x, m_rotation.y );
        glm::mat4 trans( 1 );
        trans = translate( trans, glm::vec3( 0, 0, -10 ) );

        transform.local = trans * rot;

        cCamera::update();
    }

    void cFreeFlightCamera::input( const input::sInput& _input )
    {
        if( _input.mouse_cursor.updated )
        {
            m_rotation.x -= static_cast< float >( _input.mouse_cursor.y_delta ) * .01f;
            m_rotation.y += static_cast< float >( _input.mouse_cursor.x_delta ) * .01f;
        }
    }
}
