#pragma once

#include "cCamera.h"
#include "engine/input/iPlayerController.h"

namespace df
{
	class cFreeFlightCamera : public cCamera,
							  public iPlayerController
	{
	public:
		explicit cFreeFlightCamera( const std::string& _name, float _speed, float _sensitivity );

		void update( float _delta_time ) override;
		void input( const input::sInputs& _input ) override;

	protected:
		float m_speed;
		float m_speed_multiplier;
		float m_sensitivity;

		cVector3f m_rotation;
		cVector3f m_position;
		cVector3f m_movement;
	};
}
