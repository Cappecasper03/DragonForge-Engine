#pragma once

#include "Input.h"

namespace df
{
    class iController
    {
    public:
        iController()          = default;
        virtual ~iController() = default;

        void setActive( const bool& _active ) { m_active = _active; }

        virtual void input( const input::sInput& _input ) = 0;

    protected:
        bool m_active;
    };
}
