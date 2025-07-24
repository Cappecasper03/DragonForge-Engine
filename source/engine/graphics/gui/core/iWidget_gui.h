#pragma once

#include <memory>

#include "engine/core/utils/cSmartPointers.h"
#include "engine/core/utils/Misc.h"
#include "engine/events/cEvent.h"
#include "engine/input/InputTypes.h"
#include "iWidgetStyle_gui.h"

namespace df::gui
{
	class iWidget_gui : public std::enable_shared_from_this< iWidget_gui >
	{
	public:
		DF_DefaultCopyAndMove( iWidget_gui );

		iWidget_gui();
		virtual ~iWidget_gui();

		bool isMouseInside() const { return m_inside; }
		bool isMouseButtonDown() const { return m_button_down; }

		virtual void paint() const = 0;

	protected:
		void         checkHover() const;
		void         update();
		virtual void initialize() {}

		cEvent<> m_on_mouse_button_down;
		cEvent<> m_on_mouse_button_up;

		cEvent<> m_on_mouse_enter;
		cEvent<> m_on_mouse_leave;

		bool m_inside;
		bool m_button_down;

		cShared< iWidgetStyle_gui > m_style;

	private:
		void input( const input::sInputs& _inputs );

		static void clayOnHover( Clay_ElementId _element_id, Clay_PointerData _pointer_data, intptr_t _user_data );

		bool m_entered;
		bool m_entered_last;
	};

	template< typename T >
	class iWidgetCreate_gui : public iWidget_gui
	{
	public:
		DF_DefaultCopyAndMove( iWidgetCreate_gui );

		iWidgetCreate_gui()           = default;
		~iWidgetCreate_gui() override = default;

		static cShared< T > create()
		{
			cShared< iWidgetCreate_gui > widget = MakeShared< T >();
			widget->initialize();

			return std::static_pointer_cast< T >( widget );
		}

		static cShared< T > create( cShared< T >& _assign )
		{
			cShared< iWidgetCreate_gui > widget = MakeShared< T >();
			widget->initialize();

			_assign = std::static_pointer_cast< T >( widget );
			return _assign;
		}
	};
}
