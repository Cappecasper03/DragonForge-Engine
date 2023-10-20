#include "cFreeFlightCamera.h"

namespace df
{
    cFreeFlightCamera::cFreeFlightCamera( const float& _speed )
    : m_speed( _speed ),
      m_rotation( .0f ),
      m_position( .0f )
    {}

    void cFreeFlightCamera::update( const float& /*_delta_time*/ )
    {
        cCamera::update();
    }

    void cFreeFlightCamera::input( const input::sInput& /*_input*/ )
    { }
}
