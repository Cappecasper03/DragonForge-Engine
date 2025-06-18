#include "cVerticalList_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cShared< cVerticalList_gui > cVerticalList_gui::alignment( const cWidget_gui::eVerticalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		m_data.verticalAlignment( _alignment );

		return std::static_pointer_cast< cVerticalList_gui >( shared_from_this() );
	}

	void cVerticalList_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		m_data.direction( cWidget_gui::kTopToBottom );
		m_data.widthGrow();
		m_data.heightFit();
	}
}