#pragma once

#include "engine/core/math/cVector.h"
#include "engine/core/utils/cColor.h"

namespace df
{
	struct sLight
	{
		enum eType
		{
			kAmbient,
			kDirectional,
			kPoint,
			kSpot,
			kRectangle,
		};

		cColor    color     = color::white;
		cVector3f position  = cVector3f( 0, 0, 0 );
		float     intensity = 1;
		cVector3f direction = cVector3f( 0, 0, 0 );
		float     radius    = 0;

		float spot_inner_angle = 0;
		float spot_outer_angle = 0;

		cVector2f padding0 = cVector2f( 0, 0 );
		cVector3f padding1 = cVector3f( 0, 0, 0 );
		eType     type     = kAmbient;
	};
}