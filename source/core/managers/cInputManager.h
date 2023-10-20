#pragma once

#include "core/input/Input.h"
#include "core/misc/iSingleton.h"

struct GLFWwindow;

namespace df
{
    class cInputManager final : public iSingleton< cInputManager >
    {
    public:
        DISABLE_COPY_AND_MOVE( cInputManager );

        explicit cInputManager( GLFWwindow* _window );
        ~cInputManager() override = default;

        void update();

    private:
        static void keyCallback( GLFWwindow* _window, const int _key, const int _scancode, const int _action, const int _mods );
        static void mouseButtonCallback( GLFWwindow* _window, const int _button, const int _action, const int _mods );
        static void cursorPositionCallback( GLFWwindow* _window, const double _x_position, const double _y_position );
        static void scrollCallback( GLFWwindow* _window, const double _x_offset, const double _y_offset );

        input::sInput m_input;
    };
}
