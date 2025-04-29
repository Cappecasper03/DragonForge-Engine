#include "cInputManager.h"

#include "misc/cTransform.h"
#include "profiling/ProfilingMacros.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/api/iRenderer.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"

#include "application/application/cApplication.h"
#include "cEventManager.h"
#include "events/eEventType.h"

namespace df
{
	cInputManager::cInputManager()
	{}

	cInputManager::~cInputManager()
	{}

	void cInputManager::update()
	{
		DF_ProfilingScopeCpu;

		input::sInput& input = getInstance()->m_input;

		SDL_Event event;
		while( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case eQuit:
				{
					cApplication::quit();
					break;
				}
				case eKeyDown:
				case eKeyUp:
				{
					updateInput( event.key );
					break;
				}
				case eMouseButtonDown:
				case eMouseButtonUp:
				{
					updateInput( event.button );
					break;
				}
				case eMouseMotion:
				{
					updateInput( event.motion );
					break;
				}
				case eMouseWheel:
				{
					updateInput( event.wheel );
					break;
				}
				case eWindowMinimized:
				{
					cRenderer::getRenderInstance()->setWindowMinimized( true );
					break;
				}
				case eWindowRestored:
				{
					cRenderer::getRenderInstance()->setWindowMinimized( false );
					break;
				}
				case eWindowResized:
				{
					cRenderer::getRenderInstance()->setWindowResized( true );
					break;
				}
				default:
				{
					continue;
				}
			}

			const input::sInput& const_input = input;
			cEventManager::invoke( event::input, const_input );

			input.keyboard.clear();
			input.mouse_button.clear();
			input.mouse_cursor.x_delta = 0;
			input.mouse_cursor.y_delta = 0;
			input.mouse_scroll.x_delta = 0;
			input.mouse_scroll.y_delta = 0;
		}
	}

	bool cInputManager::checkKey( const input::eKey _key, const input::eAction _action )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< unsigned, input::sKeyboard >& keyboard = getInstance()->m_input.keyboard;
		const auto                                        key_it   = keyboard.find( _key );

		if( key_it != keyboard.end() && key_it->second.action == _action )
			return true;

		return false;
	}

	input::eAction cInputManager::checkKey( const input::eKey _key )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< unsigned, input::sKeyboard >& keyboard = getInstance()->m_input.keyboard;
		const auto                                        key_it   = keyboard.find( _key );

		if( key_it != keyboard.end() )
			return static_cast< input::eAction >( key_it->second.action );

		return input::kNone;
	}

	bool cInputManager::checkButton( const input::eButton _button, const input::eAction _action )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< int, input::sMouseButton >& mouse_button = getInstance()->m_input.mouse_button;
		const auto                                      button_it    = mouse_button.find( _button );

		if( button_it != mouse_button.end() && button_it->second.action == _action )
			return true;

		return false;
	}

	input::eAction cInputManager::checkButton( const input::eButton _button )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< int, input::sMouseButton >& mouse_button = getInstance()->m_input.mouse_button;
		const auto                                      button_it    = mouse_button.find( _button );

		if( button_it != mouse_button.end() )
			return static_cast< input::eAction >( button_it->second.action );

		return input::kNone;
	}

	void cInputManager::updateInput( const SDL_KeyboardEvent& _event )
	{
		DF_ProfilingScopeCpu;

		int action;

		if( _event.repeat != 0 )
			action = input::kRepeat;
		else if( _event.down )
			action = input::kPress;
		else if( !_event.down )
			action = input::kRelease;
		else
			action = input::kNone;

		getInstance()->m_input.keyboard[ _event.key ] = {
			.key       = _event.key,
			.action    = action,
			.modifiers = _event.mod,
		};
	}

	void cInputManager::updateInput( const SDL_MouseButtonEvent& _event )
	{
		DF_ProfilingScopeCpu;

		int action;

		if( _event.down )
			action = input::kPress;
		else if( !_event.down )
			action = input::kRelease;
		else
			action = input::kNone;

		getInstance()->m_input.mouse_button[ _event.button ] = {
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
