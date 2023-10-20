#pragma once

#include "Input.h"
#include "core/managers/cEventManager.h"

namespace df
{
    class iPlayerController
    {
    public:
        iPlayerController() { cEventManager::subscribe( event::input, this, &iPlayerController::input ); }
        virtual ~iPlayerController() = default;

        void setActive( const bool& _active ) { m_active = _active; }

        virtual void input( const input::sInput& _input ) = 0;

    protected:
        bool m_active;
    };
}
