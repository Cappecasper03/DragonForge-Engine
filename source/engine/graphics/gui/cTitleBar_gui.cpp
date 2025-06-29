#include "cTitleBar_gui.h"

#include "base/cVerticalList_gui.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cTitleBar_gui::cTitleBar_gui()
		: m_moving( false )
	{}

	void cTitleBar_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		m_content = cHorizontalList_gui::create();

		m_content->color( color::gray );
		m_content->widthGrow();
		m_content->heightFixed( 30 );

		m_content->addSlot( cHorizontalList_gui::create()->widthGrow()->heightGrow() );
		m_content->addSlot( cHorizontalList_gui::create()->widthFixed( 30 )->heightGrow()->color( color::green ) );
		m_content->addSlot( cHorizontalList_gui::create()->widthFixed( 30 )->heightGrow()->color( color::blue ) );
		m_content->addSlot( cHorizontalList_gui::create()->widthFixed( 30 )->heightGrow()->color( color::red ) );

		m_content->onMouseButtonDown(
			[ this ]
			{
				if( m_content->isMouseInside() )
					m_moving = true;
			} );
		m_content->onMouseButtonUp( [ this ] { m_moving = false; } );
	}
}