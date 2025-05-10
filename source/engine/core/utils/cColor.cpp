#include "cColor.h"

namespace df
{
	cColor::cColor()
		: r( 1 )
		, g( 1 )
		, b( 1 )
		, a( 1 )
	{}

	cColor::cColor( const float _r, const float _g, const float _b, const float _a )
		: r( _r )
		, g( _g )
		, b( _b )
		, a( _a )
	{}
}
