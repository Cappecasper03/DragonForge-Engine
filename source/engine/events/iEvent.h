﻿#pragma once

#include "engine/core/utils/Misc.h"

namespace df
{
	struct iEvent
	{
		DF_DisableCopyAndMove( iEvent );

		iEvent()          = default;
		virtual ~iEvent() = default;

		virtual void unsubscribe( void* _object ) = 0;
	};
}
