#include "cLayout_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{

	cLayout_gui& cLayout_gui::widthFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.width = CLAY_SIZING_FIT( _min, _max );

		return *this;
	}

	cLayout_gui& cLayout_gui::widthFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.width = CLAY_SIZING_FIT( _min_max, _min_max );

		return *this;
	}

	cLayout_gui& cLayout_gui::widthGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.width = CLAY_SIZING_GROW( _min, _max );

		return *this;
	}

	cLayout_gui& cLayout_gui::widthGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.width = CLAY_SIZING_GROW( _min_max, _min_max );

		return *this;
	}

	cLayout_gui& cLayout_gui::widthFixed( const float _width )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.width = CLAY_SIZING_FIXED( _width );

		return *this;
	}

	cLayout_gui& cLayout_gui::widthPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.width = CLAY_SIZING_PERCENT( _percent );

		return *this;
	}

	cLayout_gui& cLayout_gui::heightFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.height = CLAY_SIZING_FIT( _min, _max );

		return *this;
	}

	cLayout_gui& cLayout_gui::heightFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.height = CLAY_SIZING_FIT( _min_max, _min_max );

		return *this;
	}

	cLayout_gui& cLayout_gui::heightGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.height = CLAY_SIZING_GROW( _min, _max );

		return *this;
	}

	cLayout_gui& cLayout_gui::heightGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.height = CLAY_SIZING_GROW( _min_max, _min_max );

		return *this;
	}

	cLayout_gui& cLayout_gui::heightFixed( const float _height )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.height = CLAY_SIZING_FIXED( _height );

		return *this;
	}

	cLayout_gui& cLayout_gui::heightPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.sizing.height = CLAY_SIZING_PERCENT( _percent );

		return *this;
	}

	cLayout_gui& cLayout_gui::padding( const std::uint16_t _left, const std::uint16_t _right, const std::uint16_t _top, const std::uint16_t _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding.left   = _left;
		m_data.padding.right  = _right;
		m_data.padding.top    = _top;
		m_data.padding.bottom = _bottom;

		return *this;
	}

	cLayout_gui& cLayout_gui::padding( const std::uint16_t _left_right, const std::uint16_t _top_bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding.left   = _left_right;
		m_data.padding.right  = _left_right;
		m_data.padding.top    = _top_bottom;
		m_data.padding.bottom = _top_bottom;

		return *this;
	}

	cLayout_gui& cLayout_gui::padding( const std::uint16_t _padding )
	{
		DF_ProfilingScopeCpu;

		m_data.padding.left   = _padding;
		m_data.padding.right  = _padding;
		m_data.padding.top    = _padding;
		m_data.padding.bottom = _padding;

		return *this;
	}

	cLayout_gui& cLayout_gui::margin( const std::uint16_t _margin )
	{
		DF_ProfilingScopeCpu;

		m_data.childGap = _margin;

		return *this;
	}

	cLayout_gui& cLayout_gui::horizontalAlignment( const eHorizontalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		switch( _alignment )
		{
			case kLeftH:   m_data.childAlignment.x = CLAY_ALIGN_X_LEFT; break;
			case kRightH:  m_data.childAlignment.x = CLAY_ALIGN_X_RIGHT; break;
			case kCenterH: m_data.childAlignment.x = CLAY_ALIGN_X_CENTER; break;
		}

		return *this;
	}

	cLayout_gui& cLayout_gui::verticalAlignment( const eVerticalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		switch( _alignment )
		{
			case kTopV:    m_data.childAlignment.y = CLAY_ALIGN_Y_TOP; break;
			case kBottomV: m_data.childAlignment.y = CLAY_ALIGN_Y_BOTTOM; break;
			case kCenterV: m_data.childAlignment.y = CLAY_ALIGN_Y_CENTER; break;
		}

		return *this;
	}

	cLayout_gui& cLayout_gui::alignment( const eHorizontalAlignment _horizontal, const eVerticalAlignment _vertical )
	{
		DF_ProfilingScopeCpu;

		horizontalAlignment( _horizontal );
		verticalAlignment( _vertical );

		return *this;
	}

	cLayout_gui& cLayout_gui::direction( const eDirection _direction )
	{
		DF_ProfilingScopeCpu;

		switch( _direction )
		{
			case kLeftToRight: m_data.layoutDirection = CLAY_LEFT_TO_RIGHT; break;
			case kTopToBottom: m_data.layoutDirection = CLAY_TOP_TO_BOTTOM; break;
		}

		return *this;
	}
}