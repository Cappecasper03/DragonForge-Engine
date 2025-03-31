#pragma once

#include <SDL3/SDL_events.h>

#include "engine/input/Input.h"
#include "engine/misc/iSingleton.h"

namespace df
{
	class cInputManager final : public iSingleton< cInputManager >
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cInputManager );

		cInputManager();
		~cInputManager() override;

		static void update();

		static bool           checkKey( int _key, input::eAction _action );
		static input::eAction checkKey( int _key );
		static bool           checkButton( int _key, input::eAction _action );
		static input::eAction checkButton( int _key );

	private:
		static void updateInput( const SDL_KeyboardEvent& _event );
		static void updateInput( const SDL_MouseButtonEvent& _event );
		static void updateInput( const SDL_MouseMotionEvent& _event );
		static void updateInput( const SDL_MouseWheelEvent& _event );

		input::sInput m_input;
	};
}
