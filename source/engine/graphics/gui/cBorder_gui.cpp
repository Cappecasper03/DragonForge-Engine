#include "cBorder_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cBorder_gui& cBorder_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.color.r = _color.r;
		m_data.color.g = _color.g;
		m_data.color.b = _color.b;
		m_data.color.a = _color.a;

		return *this;
	}

	cBorder_gui& cBorder_gui::width( const std::uint16_t _left,
	                                 const std::uint16_t _right,
	                                 const std::uint16_t _top,
	                                 const std::uint16_t _bottom,
	                                 const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.width.left            = _left;
		m_data.width.right           = _right;
		m_data.width.top             = _top;
		m_data.width.bottom          = _bottom;
		m_data.width.betweenChildren = _between_children;

		return *this;
	}

	cBorder_gui& cBorder_gui::width( const std::uint16_t _left_right, const std::uint16_t _top_bottom, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.width.left            = _left_right;
		m_data.width.right           = _left_right;
		m_data.width.top             = _top_bottom;
		m_data.width.bottom          = _top_bottom;
		m_data.width.betweenChildren = _between_children;

		return *this;
	}

	cBorder_gui& cBorder_gui::width( const std::uint16_t _radius, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.width.left            = _radius;
		m_data.width.right           = _radius;
		m_data.width.top             = _radius;
		m_data.width.bottom          = _radius;
		m_data.width.betweenChildren = _between_children;

		return *this;
	}
}