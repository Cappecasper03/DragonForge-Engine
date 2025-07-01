#include "cTitleBar_gui.h"

#include "base/cTextBlock_gui.h"
#include "base/cVerticalList_gui.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cTitleBar_gui::cTitleBar_gui()
		: m_moving( false )
		, m_button_down( false )
	{}

	bool cTitleBar_gui::wasButtonDownThisFrame()
	{
		DF_ProfilingScopeCpu;

		const bool temp = m_button_down;
		m_button_down   = false;

		return temp;
	}

	void cTitleBar_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		m_content = cHorizontalList_gui::create();

		m_content->color( color::gray );
		m_content->widthGrow();
		m_content->heightFixed( 30 );

		m_content->addSlot( cHorizontalList_gui::create()->widthGrow()->heightGrow() );
		m_content->addSlot( cTextBlock_gui::create()->text( "X" )->textColor( color::black )->backgroundColor( color::red )->widthFixed( 30 )->heightGrow() );

		m_content->onMouseButtonDown(
			[ this ]
			{
				if( m_content->isMouseInside() )
				{
					m_moving      = true;
					m_button_down = true;
				}
			} );
		m_content->onMouseButtonUp( [ this ] { m_moving = false; } );
	}
}