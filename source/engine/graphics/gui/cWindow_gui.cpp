#include "cWindow_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cShared< cWindow_gui > cWindow_gui::setContent( cShared< iWidget_gui > _content )
	{
		DF_ProfilingScopeCpu;

		return std::static_pointer_cast< cWindow_gui >( shared_from_this() );
	}

	void cWindow_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		m_data.paint();
	}

	void cWindow_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		// m_data.layout( cLayout_gui().direction( cLayout_gui::kTopToBottom ).widthFixed( 200 ).heightFixed( 250 ) );
		m_data.color( color::gray );
		m_data.floating( cFloating_gui().attachMode( cFloating_gui::kRoot ).offset( cVector2f( 200, 200 ) ) );
		m_data.border( cBorder_gui().width( 1, 0 ).color( color::purple ) );

		// m_data->addChild( cPanel_gui::create( m_title_bar )->layout( cLayout_gui().widthGrow().heightFixed( 25 ) )->color( color::gray ) );

		// m_data->addChild( cPanel_gui::create( m_content )->layout( cLayout_gui().widthGrow().heightGrow() )->color( color::black ) );
	}
}