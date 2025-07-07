#include "iMouseEvents_gui.h"

#include "engine/managers/cEventManager.h"
#include "engine/managers/cInputManager.h"

namespace df::gui
{
	iMouseEvents_gui::iMouseEvents_gui()
		: m_inside( false )
		, m_button_down( false )
		, m_entered( false )
		, m_entered_last( false )
	{
		DF_ProfilingScopeCpu;

		cEventManager::subscribe( event::input, this, &iMouseEvents_gui::input );
		cEventManager::subscribe( event::update_gui, this, &iMouseEvents_gui::update );
	}

	iMouseEvents_gui::~iMouseEvents_gui()
	{
		DF_ProfilingScopeCpu;

		cEventManager::unsubscribe( event::update_gui, this );
		cEventManager::unsubscribe( event::input, this );
	}

	void iMouseEvents_gui::checkHover() const
	{
		DF_ProfilingScopeCpu;

		Clay_OnHover( clayOnHover, reinterpret_cast< intptr_t >( this ) );
	}

	void iMouseEvents_gui::update()
	{
		DF_ProfilingScopeCpu;

		if( m_entered && !m_entered_last )
		{
			m_inside = true;
			if( m_on_mouse_enter.isBound() )
				m_on_mouse_enter.invoke();
		}

		if( !m_entered && m_entered_last )
		{
			m_inside = false;
			if( m_on_mouse_leave.isBound() )
				m_on_mouse_leave.invoke();
		}

		m_entered_last = m_entered;
		m_entered      = false;
	}

	void iMouseEvents_gui::input( const input::sInputs& /*_inputs*/ )
	{
		DF_ProfilingScopeCpu;

		if( !m_inside )
			return;

		if( cInputManager::checkButton( input::sMouseInput::kButtonLeft, input::sActionInput::kPress ) )
		{
			m_button_down = true;
			if( m_on_mouse_button_down.isBound() )
				m_on_mouse_button_down.invoke();
		}

		if( cInputManager::checkButton( input::sMouseInput::kButtonLeft, input::sActionInput::kRelease ) )
		{
			m_button_down = false;
			if( m_on_mouse_button_up.isBound() )
				m_on_mouse_button_up.invoke();
		}
	}

	void iMouseEvents_gui::clayOnHover( Clay_ElementId /*_element_id*/, Clay_PointerData /*_pointer_data*/, const intptr_t _user_data )
	{
		DF_ProfilingScopeCpu;

		iMouseEvents_gui* widget = reinterpret_cast< iMouseEvents_gui* >( _user_data );
		if( !widget )
			return;

		widget->m_entered = true;
	}
}
