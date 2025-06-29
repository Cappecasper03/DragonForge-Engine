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

		void onMouseButtonDown( void ( *_function )() );
		void onMouseButtonUp( void ( *_function )() );
		void onMouseEnter( void ( *_function )() );
		void onMouseLeave( void ( *_function )() );

	protected:
		void checkHover() const;

	private:
		void update();
		void input( const input::sInputs& _inputs );

		void tryUpdateSubscribe();
		void tryInputSubscribe();

		cEvent<> m_on_mouse_button_down;
		cEvent<> m_on_mouse_button_up;
		cEvent<> m_on_mouse_enter;
		cEvent<> m_on_mouse_leave;

		static void clayOnHover( Clay_ElementId _element_id, Clay_PointerData _pointer_data, intptr_t _user_data );

		bool m_inside;
		bool m_entered;
		bool m_entered_last;

		bool m_has_update;
		bool m_has_input;
	};
}
