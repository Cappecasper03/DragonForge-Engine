#include "cInputManager.h"

#include "application/application/cApplication.h"
#include "cEventManager.h"
#include "engine/events/eEventType.h"
#include "engine/graphics/api/iRenderer.h"
#include "engine/graphics/cRenderer.h"
#include "engine/profiling/ProfilingMacros.h"

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
				case event::sApplicationEvent::eQuit:
				{
					cApplication::quit();
					break;
				}
				case event::sKeyboardEvent::eKeyDown:
				case event::sKeyboardEvent::eKeyUp:
				{
					updateInput( event.key );
					break;
				}
				case event::sMouseEvent::eMouseButtonDown:
				case event::sMouseEvent::eMouseButtonUp:
				{
					updateInput( event.button );
					break;
				}
				case event::sMouseEvent::eMouseMotion:
				{
					updateInput( event.motion );
					break;
				}
				case event::sMouseEvent::eMouseWheel:
				{
					updateInput( event.wheel );
					break;
				}
				case event::sWindowEvent::eWindowMinimized:
				{
					cRenderer::getRenderInstance()->setWindowMinimized( true );
					break;
				}
				case event::sWindowEvent::eWindowRestored:
				{
					cRenderer::getRenderInstance()->setWindowMinimized( false );
					break;
				}
				case event::sWindowEvent::eWindowResized:
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

	bool cInputManager::checkKey( const input::sKeyboardInput::eEnum _key, const input::sActionInput::eEnum _action )
	{
		DF_ProfilingScopeCpu;

		auto&      keyboard = getInstance()->m_input.keyboard;
		const auto key_it   = keyboard.find( _key );

		if( key_it != keyboard.end() && key_it->second.action == _action )
			return true;

		return false;
	}

	input::sActionInput::eEnum cInputManager::checkKey( const input::sKeyboardInput::eEnum _key )
	{
		DF_ProfilingScopeCpu;

		auto&      keyboard = getInstance()->m_input.keyboard;
		const auto key_it   = keyboard.find( _key );

		if( key_it != keyboard.end() )
			return key_it->second.action;

		return input::sActionInput::kNone;
	}

	bool cInputManager::checkButton( const input::sMouseInput::eEnum _button, const input::sActionInput::eEnum _action )
	{
		DF_ProfilingScopeCpu;

		auto&      mouse_button = getInstance()->m_input.mouse_button;
		const auto button_it    = mouse_button.find( _button );

		if( button_it != mouse_button.end() && button_it->second.action == _action )
			return true;

		return false;
	}

	input::sActionInput::eEnum cInputManager::checkButton( const input::sMouseInput::eEnum _button )
	{
		DF_ProfilingScopeCpu;

		auto&      mouse_button = getInstance()->m_input.mouse_button;
		const auto button_it    = mouse_button.find( _button );

		if( button_it != mouse_button.end() )
			return button_it->second.action;

		return input::sActionInput::kNone;
	}

	void cInputManager::updateInput( const SDL_KeyboardEvent& _event )
	{
		DF_ProfilingScopeCpu;

		input::sActionInput::eEnum action;

		if( _event.repeat != 0 )
			action = input::sActionInput::kRepeat;
		else if( _event.down )
			action = input::sActionInput::kPress;
		else if( !_event.down )
			action = input::sActionInput::kRelease;
		else
			action = input::sActionInput::kNone;

		getInstance()->m_input.keyboard[ static_cast< input::sKeyboardInput::eEnum >( _event.key ) ] = {
			.key       = static_cast< input::sKeyboardInput::eEnum >( _event.key ),
			.action    = ( action ),
			.modifiers = static_cast< input::sInput< input::kKeyModifier >::eEnum >( _event.mod ),
		};
	}

	void cInputManager::updateInput( const SDL_MouseButtonEvent& _event )
	{
		DF_ProfilingScopeCpu;

		input::sActionInput::eEnum action;

		if( _event.down )
			action = input::sActionInput::kPress;
		else if( !_event.down )
			action = input::sActionInput::kRelease;
		else
			action = input::sActionInput::kNone;

		getInstance()->m_input.mouse_button[ static_cast< input::sMouseInput::eEnum >( _event.button ) ] = {
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
