#pragma once

#include "engine/input/Input.h"
#include "engine/misc/iSingleton.h"

struct GLFWwindow;

namespace df
{
	class cInputManager final : public iSingleton< cInputManager >
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cInputManager );

		cInputManager();
		~cInputManager() override = default;

		static void update();

		static bool           checkKey( const int& _key, const int& _action );
		static input::eAction checkKey( const int& _key );
		static bool           checkButton( const int& _key, const int& _action );
		static input::eAction checkButton( const int& _key );

	private:
		static void keyCallback( GLFWwindow* _window, const int _key, const int _scancode, const int _action, const int _mods );
		static void mouseButtonCallback( GLFWwindow* _window, const int _button, const int _action, const int _mods );
		static void cursorPositionCallback( GLFWwindow* _window, const double _x_position, const double _y_position );
		static void scrollCallback( GLFWwindow* _window, const double _x_offset, const double _y_offset );
		static void cursorEnterCallback( GLFWwindow* _window, const int _entered );

		input::sInput m_input;
	};
}
