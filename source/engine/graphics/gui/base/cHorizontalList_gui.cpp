#include "cHorizontalList_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cShared< cHorizontalList_gui > cHorizontalList_gui::alignment( const cWidget_gui::eHorizontalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		m_data.horizontalAlignment( _alignment );

		return std::static_pointer_cast< cHorizontalList_gui >( shared_from_this() );
	}

	void cHorizontalList_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		m_data.direction( cWidget_gui::kLeftToRight );
		m_data.verticalAlignment( cWidget_gui::kCenterV );
		m_data.widthFit();
		m_data.heightGrow();
	}
}