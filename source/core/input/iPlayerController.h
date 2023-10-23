#pragma once

#include "Input.h"
#include "core/managers/cEventManager.h"

namespace df
{
    class iPlayerController
    {
    public:
        DISABLE_COPY_AND_MOVE( iPlayerController );

        iPlayerController() { cEventManager::subscribe( event::input, this, &iPlayerController::input ); }
        virtual ~iPlayerController() = default;

        void setActive( const bool& _active ) { m_active = _active; }

        virtual void update( const float& /*_delta_time*/ ) {}
        virtual void input( const input::sInput& _input ) = 0;

    protected:
        bool m_active;
    };
}
