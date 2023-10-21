#include "cInputManager.h"

#include <GLFW/glfw3.h>

#include "cEventManager.h"

namespace df
{
    cInputManager::cInputManager( GLFWwindow* _window )
    {
        glfwSetKeyCallback( _window, keyCallback );
        glfwSetMouseButtonCallback( _window, mouseButtonCallback );
        glfwSetCursorPosCallback( _window, cursorPositionCallback );
        glfwSetScrollCallback( _window, scrollCallback );
        glfwSetCursorEnterCallback( _window, cursorEnterCallback );
    }

    void cInputManager::update()
    {
        glfwPollEvents();
        cEventManager::invoke( event::input, m_input );

        m_input.keyboard.clear();
        m_input.mouse_button.clear();
        m_input.mouse_cursor.updated = false;
        m_input.mouse_scroll.updated = false;
    }

    bool cInputManager::checkKey( const int& _key, const int& _action )
    {
        std::map< int, input::sKeyboard >& keyboard = getInstance()->m_input.keyboard;
        const auto                         key_it   = keyboard.find( _key );

        if( key_it != keyboard.end() && key_it->second.action == _action )
            return true;

        return false;
    }

    bool cInputManager::checkButton( const int& _key, const int& _action )
    {
        std::map< int, input::sMouseButton >& mouse_button = getInstance()->m_input.mouse_button;
        const auto                            button_it    = mouse_button.find( _key );

        if( button_it != mouse_button.end() && button_it->second.action == _action )
            return true;

        return false;
    }

    void cInputManager::keyCallback( GLFWwindow* /*_window*/, const int _key, const int _scancode, const int _action, const int _mods )
    {
        getInstance()->m_input.keyboard[ _key ] = { _scancode, _action, _mods };
    }

    void cInputManager::mouseButtonCallback( GLFWwindow* /*_window*/, const int _button, const int _action, const int _mods )
    {
        getInstance()->m_input.mouse_button[ _button ] = { _action, _mods };
    }

    void cInputManager::cursorPositionCallback( GLFWwindow* /*_window*/, const double _x_position, const double _y_position )
    {
        input::sMouseCursor& mouse_cursor = getInstance()->m_input.mouse_cursor;

        mouse_cursor.updated = true;

        if( mouse_cursor.on_window_current && mouse_cursor.on_window_previus )
        {
            mouse_cursor.x_delta = mouse_cursor.x_current - mouse_cursor.x_previus;
            mouse_cursor.y_delta = mouse_cursor.y_current - mouse_cursor.y_previus;

            mouse_cursor.x_previus = mouse_cursor.x_current;
            mouse_cursor.y_previus = mouse_cursor.y_current;
        }
        else
        {
            mouse_cursor.x_previus = _x_position;
            mouse_cursor.y_previus = _y_position;
        }

        mouse_cursor.x_current = _x_position;
        mouse_cursor.y_current = _y_position;

        mouse_cursor.on_window_previus = mouse_cursor.on_window_current;
    }

    void cInputManager::scrollCallback( GLFWwindow* /*_window*/, const double _x_offset, const double _y_offset )
    {
        input::sMouseScroll& mouse_scroll = getInstance()->m_input.mouse_scroll;

        mouse_scroll.updated = true;

        mouse_scroll.x_offset = _x_offset;
        mouse_scroll.y_offset = _y_offset;
    }

    void cInputManager::cursorEnterCallback( GLFWwindow* _window, const int _entered )
    {
        input::sMouseCursor& mouse_cursor = getInstance()->m_input.mouse_cursor;

        mouse_cursor.on_window_previus = mouse_cursor.on_window_current;
        mouse_cursor.on_window_current = _entered;
    }
}
