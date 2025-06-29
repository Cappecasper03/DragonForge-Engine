#include "iMouseEvents_gui.h"

#include "engine/managers/cEventManager.h"
#include "engine/managers/cInputManager.h"

namespace df::gui
{
	iMouseEvents_gui::iMouseEvents_gui()
		: m_inside( false )
		, m_entered( false )
		, m_entered_last( false )
		, m_has_update( false )
		, m_has_input( false )
	{}

	void iMouseEvents_gui::onMouseButtonDown( void ( *_function )() )
	{
		DF_ProfilingScopeCpu;

		tryInputSubscribe();

		m_on_mouse_button_down.subscribe( _function );
	}

	void iMouseEvents_gui::onMouseButtonUp( void ( *_function )() )
	{
		DF_ProfilingScopeCpu;

		tryInputSubscribe();

		m_on_mouse_button_up.subscribe( _function );
	}

	void iMouseEvents_gui::onMouseEnter( void ( *_function )() )
	{
		DF_ProfilingScopeCpu;

		tryUpdateSubscribe();

		m_on_mouse_enter.subscribe( _function );
	}

	void iMouseEvents_gui::onMouseLeave( void ( *_function )() )
	{
		DF_ProfilingScopeCpu;

		tryUpdateSubscribe();

		m_on_mouse_leave.subscribe( _function );
	}

	void iMouseEvents_gui::checkHover() const
	{
		DF_ProfilingScopeCpu;

		if( m_has_update || m_has_input )
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
			m_on_mouse_button_down.invoke();

		if( cInputManager::checkButton( input::sMouseInput::kButtonLeft, input::sActionInput::kRelease ) )
			m_on_mouse_button_up.invoke();
	}

	void iMouseEvents_gui::tryUpdateSubscribe()
	{
		DF_ProfilingScopeCpu;

		if( m_has_update )
			return;

		cEventManager::subscribe( event::update_gui, this, &iMouseEvents_gui::update );
		m_has_update = true;
	}

	void iMouseEvents_gui::tryInputSubscribe()
	{
		DF_ProfilingScopeCpu;

		if( m_has_input )
			return;

		cEventManager::subscribe( event::input, this, &iMouseEvents_gui::input );
		m_has_input = true;
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
