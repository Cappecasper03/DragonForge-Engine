#pragma once

#include "engine/core/utils/Misc.h"

namespace df
{
	struct iEvent
	{
		DF_DeleteCopyAndMove( iEvent );

		iEvent()          = default;
		virtual ~iEvent() = default;

		virtual void unsubscribe( void* _object ) = 0;
	};
}
