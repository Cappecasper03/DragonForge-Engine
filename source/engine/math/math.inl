#pragma once

#include <glm/trigonometric.hpp>

namespace df::math
{
	template< typename T >
	T radians( T _degrees )
	{
		return glm::radians( _degrees );
	}
}