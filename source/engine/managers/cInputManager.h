#pragma once

#include <SDL3/SDL_events.h>

#include "engine/input/InputTypes.h"
#include "engine/misc/iSingleton.h"

namespace df
{
	class cInputManager final : public iSingleton< cInputManager >
	{
	public:
		DF_DisableCopyAndMove( cInputManager );

		cInputManager();
		~cInputManager() override;

		static void update();

		static bool           checkKey( input::eKey _key, input::eAction _action );
		static input::eAction checkKey( input::eKey _key );
		static bool           checkButton( input::eButton _button, input::eAction _action );
		static input::eAction checkButton( input::eButton _button );

	private:
		static void updateInput( const SDL_KeyboardEvent& _event );
		static void updateInput( const SDL_MouseButtonEvent& _event );
		static void updateInput( const SDL_MouseMotionEvent& _event );
		static void updateInput( const SDL_MouseWheelEvent& _event );

		input::sInput m_input;
	};
}
