#pragma once

#include <clay.h>

#include "engine/core/utils/Misc.h"
#include "engine/events/cEvent.h"
#include "engine/input/InputTypes.h"

namespace df::gui
{
	class iMouseEvents_gui
	{
	public:
		DF_DefaultCopyAndMove( iMouseEvents_gui );

		iMouseEvents_gui();
		virtual ~iMouseEvents_gui() = default;

		void onMouseButtonDown( const std::function< void() >& _function ) { m_on_mouse_button_down.subscribe( _function ); }
		void onMouseButtonUp( const std::function< void() >& _function ) { m_on_mouse_button_up.subscribe( _function ); }

		void onMouseEnter( const std::function< void() >& _function ) { m_on_mouse_enter.subscribe( _function ); }
		void onMouseLeave( const std::function< void() >& _function ) { m_on_mouse_leave.subscribe( _function ); }

		bool isMouseInside() const { return m_inside; }
		bool isMouseButtonDown() const { return m_button_down; }

	protected:
		void checkHover() const;

	private:
		void update();
		void input( const input::sInputs& _inputs );

		cEvent<> m_on_mouse_button_down;
		cEvent<> m_on_mouse_button_up;

		cEvent<> m_on_mouse_enter;
		cEvent<> m_on_mouse_leave;

		static void clayOnHover( Clay_ElementId _element_id, Clay_PointerData _pointer_data, intptr_t _user_data );

		bool m_inside;
		bool m_button_down;

		bool m_entered;
		bool m_entered_last;
	};
}
