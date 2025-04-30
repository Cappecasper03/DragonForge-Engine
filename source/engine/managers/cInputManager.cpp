#include "cInputManager.h"

#include "engine/graphics/api/iRenderer.h"
#include "engine/graphics/cRenderer.h"
#include "profiling/ProfilingMacros.h"

#include "application/application/cApplication.h"
#include "cEventManager.h"
#include "events/eEventType.h"

namespace df
{
	void cInputManager::update()
	{
		DF_ProfilingScopeCpu;

		input::sInputs& input = getInstance()->m_input;

		SDL_Event event;
		while( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case event::sEvent< event::kApplication >::eQuit:
				{
					cApplication::quit();
					break;
				}
				case event::sEvent< event::kKeyboard >::eEnum::eKeyDown:
				case event::sEvent< event::kKeyboard >::eKeyUp:
				{
					updateInput( event.key );
					break;
				}
				case event::sEvent< event::kMouse >::eMouseButtonDown:
				case event::sEvent< event::kMouse >::eMouseButtonUp:
				{
					updateInput( event.button );
					break;
				}
				case event::sEvent< event::kMouse >::eMouseMotion:
				{
					updateInput( event.motion );
					break;
				}
				case event::sEvent< event::kMouse >::eMouseWheel:
				{
					updateInput( event.wheel );
					break;
				}
				case event::sEvent< event::kWindow >::eWindowMinimized:
				{
					cRenderer::getRenderInstance()->setWindowMinimized( true );
					break;
				}
				case event::sEvent< event::kWindow >::eWindowRestored:
				{
					cRenderer::getRenderInstance()->setWindowMinimized( false );
					break;
				}
				case event::sEvent< event::kWindow >::eWindowResized:
				{
					cRenderer::getRenderInstance()->setWindowResized( true );
					break;
				}
				default:
				{
					continue;
				}
			}

			const input::sInputs& const_input = input;
			cEventManager::invoke( event::input, const_input );

			input.keyboard.clear();
			input.mouse_button.clear();
			input.mouse_cursor.x_delta = 0;
			input.mouse_cursor.y_delta = 0;
			input.mouse_scroll.x_delta = 0;
			input.mouse_scroll.y_delta = 0;
		}
	}

	bool cInputManager::checkKey( const input::sInput< input::kKeyboard >::eEnum _key, const input::sInput< input::kAction >::eEnum _action )
	{
		DF_ProfilingScopeCpu;

		auto&      keyboard = getInstance()->m_input.keyboard;
		const auto key_it   = keyboard.find( _key );

		if( key_it != keyboard.end() && key_it->second.action == _action )
			return true;

		return false;
	}

	input::sInput< input::kAction >::eEnum cInputManager::checkKey( const input::sInput< input::kKeyboard >::eEnum _key )
	{
		DF_ProfilingScopeCpu;

		auto&      keyboard = getInstance()->m_input.keyboard;
		const auto key_it   = keyboard.find( _key );

		if( key_it != keyboard.end() )
			return key_it->second.action;

		return input::sInput< input::kAction >::kNone;
	}

	bool cInputManager::checkButton( const input::sInput< input::kMouse >::eEnum _button, const input::sInput< input::kAction >::eEnum _action )
	{
		DF_ProfilingScopeCpu;

		auto&      mouse_button = getInstance()->m_input.mouse_button;
		const auto button_it    = mouse_button.find( _button );

		if( button_it != mouse_button.end() && button_it->second.action == _action )
			return true;

		return false;
	}

	input::sInput< input::kAction >::eEnum cInputManager::checkButton( const input::sInput< input::kMouse >::eEnum _button )
	{
		DF_ProfilingScopeCpu;

		auto&      mouse_button = getInstance()->m_input.mouse_button;
		const auto button_it    = mouse_button.find( _button );

		if( button_it != mouse_button.end() )
			return button_it->second.action;

		return input::sInput< input::kAction >::kNone;
	}

	void cInputManager::updateInput( const SDL_KeyboardEvent& _event )
	{
		DF_ProfilingScopeCpu;

		input::sInput< input::kAction >::eEnum action;

		if( _event.repeat != 0 )
			action = input::sInput< input::kAction >::kRepeat;
		else if( _event.down )
			action = input::sInput< input::kAction >::kPress;
		else if( !_event.down )
			action = input::sInput< input::kAction >::kRelease;
		else
			action = input::sInput< input::kAction >::kNone;

		getInstance()->m_input.keyboard[ static_cast< input::sInput< input::kKeyboard >::eEnum >( _event.key ) ] = {
			.key       = static_cast< input::sInput< input::kKeyboard >::eEnum >( _event.key ),
			.action    = ( action ),
			.modifiers = static_cast< input::sInput< input::kKeyModifier >::eEnum >( _event.mod ),
		};
	}

	void cInputManager::updateInput( const SDL_MouseButtonEvent& _event )
	{
		DF_ProfilingScopeCpu;

		input::sInput< input::kAction >::eEnum action;

		if( _event.down )
			action = input::sInput< input::kAction >::kPress;
		else if( !_event.down )
			action = input::sInput< input::kAction >::kRelease;
		else
			action = input::sInput< input::kAction >::kNone;

		getInstance()->m_input.mouse_button[ static_cast< input::sInput< input::kMouse >::eEnum >( _event.button ) ] = {
			.action = action,
			.clicks = _event.clicks,
		};
	}

	void cInputManager::updateInput( const SDL_MouseMotionEvent& _event )
	{
		DF_ProfilingScopeCpu;

		input::sMouseCursor& mouse_cursor = getInstance()->m_input.mouse_cursor;

		mouse_cursor.x_delta = _event.xrel;
		mouse_cursor.y_delta = _event.yrel;

		mouse_cursor.x_previous = mouse_cursor.x_current;
		mouse_cursor.y_previous = mouse_cursor.y_current;

		mouse_cursor.x_current = _event.x;
		mouse_cursor.y_current = _event.y;
	}

	void cInputManager::updateInput( const SDL_MouseWheelEvent& _event )
	{
		DF_ProfilingScopeCpu;

		input::sMouseScroll& mouse_scroll = getInstance()->m_input.mouse_scroll;

		mouse_scroll.x_delta = _event.x;
		mouse_scroll.y_delta = _event.y;
	}
}
