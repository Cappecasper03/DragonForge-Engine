#pragma once

#include <clay.h>
#include <cstdint>

#include "engine/core/utils/cColor.h"
#include "engine/core/utils/Misc.h"

namespace df::gui
{
	class cBorder_gui
	{
	public:
		DF_DefaultCopyAndMove( cBorder_gui );

		cBorder_gui()  = default;
		~cBorder_gui() = default;

		cBorder_gui& color( const cColor& _color );

		cBorder_gui& width( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom, std::uint16_t _between_children );
		cBorder_gui& width( std::uint16_t _left_right, std::uint16_t _top_bottom, std::uint16_t _between_children );
		cBorder_gui& width( std::uint16_t _width, std::uint16_t _between_children );

		const Clay_BorderElementConfig& get() const { return m_data; }

	private:
		Clay_BorderElementConfig m_data;
	};
}
