#include "cFreeFlightCamera.h"

#include <glm/gtc/quaternion.hpp>

#include "engine/managers/cInputManager.h"
#include "engine/misc/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cFreeFlightCamera::cFreeFlightCamera( std::string _name, const float _speed, const float _sensitivity )
		: cCamera( std::move( _name ), ePerspective, cColor( .5f, .75f, 1, 1 ), 90 )
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
			m_position                          += cVector3f( transform->world[ 0 ] ) * normalized_movement.x() * m_speed * m_speed_multiplier * _delta_time;
			m_position                          += cVector3f( transform->world[ 2 ] ) * normalized_movement.z() * m_speed * m_speed_multiplier * _delta_time;
		}

		const glm::quat yaw_quaternion   = glm::angleAxis( glm::radians( m_rotation.x() ), cVector3f( 1, 0, 0 ).getGLM() );
		const glm::quat pitch_quaternion = glm::angleAxis( glm::radians( m_rotation.y() ), cVector3f( 0, 1, 0 ).getGLM() );
		const glm::mat4 rotation( mat4_cast( pitch_quaternion * yaw_quaternion ) );

		glm::mat4 translation( 1 );
		translation = translate( translation, m_position.getGLM() );

		transform->local = translation * rotation;

		cCamera::update();
	}

	void cFreeFlightCamera::input( const input::sInput& _input )
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

		if( const input::eAction action = cInputManager::checkKey( input::eKey::kW ); action != input::kRepeat )
			m_movement.z() -= static_cast< float >( action );

		if( const input::eAction action = cInputManager::checkKey( input::eKey::kS ); action != input::kRepeat )
			m_movement.z() += static_cast< float >( action );

		if( const input::eAction action = cInputManager::checkKey( input::eKey::kA ); action != input::kRepeat )
			m_movement.x() -= static_cast< float >( action );

		if( const input::eAction action = cInputManager::checkKey( input::eKey::kD ); action != input::kRepeat )
			m_movement.x() += static_cast< float >( action );
	}
}
