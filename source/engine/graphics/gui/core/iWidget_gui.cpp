#include "iWidget_gui.h"

#include "engine/managers/cEventManager.h"
#include "engine/managers/cInputManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	iWidget_gui::iWidget_gui()
		: m_inside( false )
		, m_button_down( false )
		, m_entered( false )
		, m_entered_last( false )
	{
		DF_ProfilingScopeCpu;

		cEventManager::subscribe( event::input, this, &iWidget_gui::input );
		cEventManager::subscribe( event::update_gui, this, &iWidget_gui::update );
	}

	iWidget_gui::~iWidget_gui()
	{
		DF_ProfilingScopeCpu;

		cEventManager::unsubscribe( event::update_gui, this );
		cEventManager::unsubscribe( event::input, this );
	}

	void iWidget_gui::checkHover() const
	{
		DF_ProfilingScopeCpu;

		Clay_OnHover( clayOnHover, reinterpret_cast< intptr_t >( this ) );
	}

	void iWidget_gui::update()
	{
		DF_ProfilingScopeCpu;

		if( m_entered && !m_entered_last )
		{
			m_inside = true;
			m_style->update( shared_from_this() );
			if( m_on_mouse_enter.isBound() )
				m_on_mouse_enter.invoke();
		}

		if( !m_entered && m_entered_last )
		{
			m_inside      = false;
			m_button_down = false;
			m_style->update( shared_from_this() );
			if( m_on_mouse_leave.isBound() )
				m_on_mouse_leave.invoke();
		}

		m_entered_last = m_entered;
		m_entered      = false;
	}

	void iWidget_gui::input( const input::sInputs& _inputs )
	{
		DF_ProfilingScopeCpu;

		if( !m_inside )
			return;

		if( cInputManager::checkButton( input::sMouseInput::kButtonLeft, input::sActionInput::kPress ) )
		{
			m_button_down = true;
			m_style->update( shared_from_this() );
			if( m_on_mouse_button_down.isBound() )
				m_on_mouse_button_down.invoke();
		}

		if( cInputManager::checkButton( input::sMouseInput::kButtonLeft, input::sActionInput::kRelease ) )
		{
			m_button_down = false;
			m_style->update( shared_from_this() );
			if( m_on_mouse_button_up.isBound() )
				m_on_mouse_button_up.invoke();
		}
	}

	void iWidget_gui::clayOnHover( Clay_ElementId /*_element_id*/, Clay_PointerData /*_pointer_data*/, const intptr_t _user_data )
	{
		DF_ProfilingScopeCpu;

		iWidget_gui* widget = reinterpret_cast< iWidget_gui* >( _user_data );
		if( !widget )
			return;

		widget->m_entered = true;
	}
}