#include "cFreeFlightCamera.h"

#include "engine/core/math/cQuaternion.h"
#include "engine/core/math/math.h"
#include "engine/core/utils/cTransform.h"
#include "engine/managers/cInputManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cFreeFlightCamera::cFreeFlightCamera( const std::string& _name, const float _speed, const float _sensitivity )
		: cCamera( _name, kPerspective, cColor( .5f, .75f, 1, 1 ), 90 )
		, m_speed( _speed )
		, m_speed_multiplier( 1 )
		, m_sensitivity( _sensitivity )
		, m_rotation( .0f )
		, m_position( .0f )
		, m_movement( .0f )
	{}

	void cFreeFlightCamera::update( const float _delta_time )
	{
		DF_ProfilingScopeCpu;

		if( m_movement.x() != 0.f || m_movement.z() != 0.f )
		{
			const cVector3f normalized_movement  = m_movement.normalized();
			m_position                          += cVector3f( m_transform.m_world.right() ) * normalized_movement.x() * m_speed * m_speed_multiplier * _delta_time;
			m_position                          += cVector3f( m_transform.m_world.backward() ) * normalized_movement.z() * m_speed * m_speed_multiplier * _delta_time;
		}

		const cQuaternionf yaw_quaternion   = cQuaternionf::fromAngleAxis( math::radians( m_rotation.x() ), cVector3f( 1, 0, 0 ) );
		const cQuaternionf pitch_quaternion = cQuaternionf::fromAngleAxis( math::radians( m_rotation.y() ), cVector3f( 0, 1, 0 ) );
		const cMatrix4f    rotation         = cMatrix4f( ( pitch_quaternion * yaw_quaternion ).toMatrix() );

		cMatrix4f translation;
		translation.translate( m_position );

		m_transform.m_local = translation * rotation;

		cCamera::update();
	}

	void cFreeFlightCamera::input( const input::sInputs& _input )
	{
		DF_ProfilingScopeCpu;

		m_rotation.x() -= static_cast< float >( _input.mouse_cursor.y_delta ) * m_sensitivity;
		m_rotation.y() -= static_cast< float >( _input.mouse_cursor.x_delta ) * m_sensitivity;

		if( _input.mouse_scroll.y_delta > 0 )
			m_speed_multiplier *= 2;
		else if( _input.mouse_scroll.y_delta < 0 )
			m_speed_multiplier *= .5f;

		if( m_speed_multiplier <= 0 )
			m_speed_multiplier = 1;

		if( const input::sInput< input::kAction >::eEnum action = cInputManager::checkKey( input::sInput< input::kKeyboard >::eEnum::kW );
		    action != input::sInput< input::kAction >::eEnum::kRepeat )
			m_movement.z() -= static_cast< float >( action );

		if( const input::sInput< input::kAction >::eEnum action = cInputManager::checkKey( input::sInput< input::kKeyboard >::eEnum::kS );
		    action != input::sInput< input::kAction >::eEnum::kRepeat )
			m_movement.z() += static_cast< float >( action );

		if( const input::sInput< input::kAction >::eEnum action = cInputManager::checkKey( input::sInput< input::kKeyboard >::eEnum::kA );
		    action != input::sInput< input::kAction >::eEnum::kRepeat )
			m_movement.x() -= static_cast< float >( action );

		if( const input::sInput< input::kAction >::eEnum action = cInputManager::checkKey( input::sInput< input::kKeyboard >::eEnum::kD );
		    action != input::sInput< input::kAction >::eEnum::kRepeat )
			m_movement.x() += static_cast< float >( action );
	}
}
