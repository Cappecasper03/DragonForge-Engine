#include "iWidgetStyle_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	void iWidgetStyle_gui::applyBrush( const sWidgetBrush& _brush )
	{
		DF_ProfilingScopeCpu;

		if( _brush.image )
		{
			if( _brush.image_size.x() > 0 && _brush.image_size.y() > 0 )
				m_data.image( _brush.image.get(), _brush.image_size );
			else
				m_data.image( _brush.image.get() );
		}

		m_data.color( _brush.color );
		m_data.padding( _brush.padding );
		m_data.borderColor( _brush.border_color );
		m_data.borderWidth( _brush.border_width );
		m_data.cornerRadius( _brush.corner_radius );
	}
}
