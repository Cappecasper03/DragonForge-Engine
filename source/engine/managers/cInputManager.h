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

		cInputManager()           = default;
		~cInputManager() override = default;

		static void update();

		static bool                                   checkKey( input::sInput< input::kKeyboard >::eEnum _key, input::sInput< input::kAction >::eEnum _action );
		static input::sInput< input::kAction >::eEnum checkKey( input::sInput< input::kKeyboard >::eEnum _key );
		static bool                                   checkButton( input::sInput< input::kMouse >::eEnum _button, input::sInput< input::kAction >::eEnum _action );
		static input::sInput< input::kAction >::eEnum checkButton( input::sInput< input::kMouse >::eEnum _button );

	private:
		static void updateInput( const SDL_KeyboardEvent& _event );
		static void updateInput( const SDL_MouseButtonEvent& _event );
		static void updateInput( const SDL_MouseMotionEvent& _event );
		static void updateInput( const SDL_MouseWheelEvent& _event );

		input::sInputs m_input;
	};
}
