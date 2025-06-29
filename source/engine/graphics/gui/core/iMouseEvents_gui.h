#pragma once

#include <clay.h>

#include "engine/core/utils/Misc.h"
#include "engine/events/cEvent.h"

namespace df::gui
{
	class iMouseEvents_gui
	{
	public:
		DF_DefaultCopyAndMove( iMouseEvents_gui );

		iMouseEvents_gui();
		virtual ~iMouseEvents_gui() = default;

		void onMouseEnter( void ( *_function )() );
		void onMouseExit( void ( *_function )() );

	protected:
		void checkHover() const;

	private:
		void update();

		cEvent<> m_on_mouse_enter;
		cEvent<> m_on_mouse_exit;

		static void clayOnHover( Clay_ElementId _element_id, Clay_PointerData _pointer_data, intptr_t _user_data );

		bool m_inside;
		bool m_entered;
		bool m_entered_last;
	};
}
