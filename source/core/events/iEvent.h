﻿#pragma once

#include "core/misc/Misc.h"

namespace df
{
    class iEvent
    {
    public:
        DISABLE_COPY_AND_MOVE( iEvent );

        iEvent()          = default;
        virtual ~iEvent() = default;

        virtual void unsubscribe( void* _object ) = 0;
    };
}
