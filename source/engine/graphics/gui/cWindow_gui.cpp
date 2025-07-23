#include "cWindow_gui.h"

#include "base/cHorizontalList_gui.h"
#include "engine/managers/cEventManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cWindow_gui::cWindow_gui()
		: m_is_dragging( false )
		, m_is_open( true )
	{}

	cShared< cWindow_gui > cWindow_gui::setTitleBar( const cShared< cTitleBar_gui >& _widget )
	{
		DF_ProfilingScopeCpu;

		m_title_bar = _widget;

		return std::static_pointer_cast< cWindow_gui >( shared_from_this() );
	}

	cShared< cWindow_gui > cWindow_gui::setContent( const cShared< iWidget_gui >& _widget )
	{
		DF_ProfilingScopeCpu;

		m_content->setSlot( 1, _widget );

		return std::static_pointer_cast< cWindow_gui >( shared_from_this() );
	}

	void cWindow_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		if( m_is_open )
			m_overlay->paint();
	}

	void cWindow_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		cEventManager::subscribe( event::input,
		                          this,
		                          std::function< void( const input::sInputs& _input ) >(
									  [ this ]( const input::sInputs& _input )
									  {
										  m_mouse_position = cVector2f( _input.mouse_cursor.x_current, _input.mouse_cursor.y_current );

										  if( m_is_dragging )
											  m_overlay->floatingOffset( m_mouse_position + m_mouse_offset );
									  } ) );

		m_title_bar = cTitleBar_gui::create();
		m_title_bar->onDragStart(
			[ this ]
			{
				m_mouse_offset = m_overlay->getFloatingOffset() - cVector2f( m_mouse_position.x(), m_mouse_position.y() );
				m_is_dragging  = true;
			} );
		m_title_bar->onDragEnd( [ this ] { m_is_dragging = false; } );

		m_title_bar->onClose( [ this ] { m_is_open = false; } );

		m_overlay = cOverlay_gui::create();
		m_overlay->setContent( cVerticalList_gui::create( m_content )
		                           ->color( color::sky_blue )
		                           ->heightGrow()
		                           ->addSlot( m_title_bar )

		                           ->addSlot( nullptr ) );

		m_overlay->widthFixed( 200 );
		m_overlay->heightFixed( 300 );

		m_overlay->padding( 5 );
		m_overlay->borderWidth( 5, 0 );
		m_overlay->borderColor( color::black );

		m_overlay->floatingAttachMode( cWidget_gui::kRoot );
		m_overlay->floatingOffset( cVector2f( 200, 200 ) );
	}
}