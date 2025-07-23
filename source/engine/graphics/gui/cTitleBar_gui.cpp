#include "cTitleBar_gui.h"

#include "base/cButton_gui.h"
#include "base/cTextBlock_gui.h"
#include "base/cVerticalList_gui.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	void cTitleBar_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		m_content = cHorizontalList_gui::create();

		m_content->color( color::gray );
		m_content->widthGrow();
		m_content->heightFixed( 30 );

		m_content->addSlot( cHorizontalList_gui::create()->widthGrow()->heightGrow() );
		m_content->addSlot( cButton_gui::create()->widthFixed( 30 )->color( color::red )->setContent( cTextBlock_gui::create()->text( "X" )->textColor( color::black ) ) );

		m_content->onMouseButtonDown(
			[ this ]
			{
				if( m_content->isMouseInside() )
				{
					if( m_on_drag_start.isBound() )
						m_on_drag_start.invoke();
				}
			} );

		m_content->onMouseButtonUp(
			[ this ]
			{
				if( m_on_drag_end.isBound() )
					m_on_drag_end.invoke();
			} );
	}
}