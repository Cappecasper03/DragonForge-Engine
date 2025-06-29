#include "iMouseEvents_gui.h"

#include "engine/managers/cEventManager.h"

namespace df::gui
{
	iMouseEvents_gui::iMouseEvents_gui()
		: m_inside( false )
		, m_entered( false )
		, m_entered_last( false )
	{}

	void iMouseEvents_gui::onMouseEnter( void ( *_function )() )
	{
		DF_ProfilingScopeCpu;

		if( m_on_mouse_enter.isBound() && m_on_mouse_exit.isBound() )
			cEventManager::subscribe( event::update_gui, this, &iMouseEvents_gui::update );

		m_on_mouse_enter.subscribe( _function );
	}

	void iMouseEvents_gui::onMouseExit( void ( *_function )() )
	{
		DF_ProfilingScopeCpu;

		if( m_on_mouse_enter.isBound() && m_on_mouse_exit.isBound() )
			cEventManager::subscribe( event::update_gui, this, &iMouseEvents_gui::update );

		m_on_mouse_exit.subscribe( _function );
	}

	void iMouseEvents_gui::checkHover() const
	{
		DF_ProfilingScopeCpu;

		if( !m_on_mouse_enter.isBound() || !m_on_mouse_exit.isBound() )
			Clay_OnHover( clayOnHover, reinterpret_cast< intptr_t >( this ) );
	}

	void iMouseEvents_gui::update()
	{
		DF_ProfilingScopeCpu;

		if( m_on_mouse_enter.isBound() && m_entered && !m_entered_last )
		{
			m_inside = true;
			m_on_mouse_enter.invoke();
		}

		if( m_on_mouse_exit.isBound() && !m_entered && m_entered_last )
		{
			m_inside = false;
			m_on_mouse_exit.invoke();
		}

		m_entered_last = m_entered;
		m_entered      = false;
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
