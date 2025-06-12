#pragma once

#include <clay.h>
#include <cstdint>

#include "engine/core/utils/Misc.h"

namespace df::gui
{
	class cLayout_gui
	{
	public:
		DF_DefaultCopyAndMove( cLayout_gui );

		enum eHorizontalAlignment : std::uint8_t
		{
			kLeftH,
			kRightH,
			kCenterH,
		};

		enum eVerticalAlignment : std::uint8_t
		{
			kTopV,
			kBottomV,
			kCenterV,
		};

		enum eDirection : std::uint8_t
		{
			kLeftToRight,
			kTopToBottom,
		};

		cLayout_gui()  = default;
		~cLayout_gui() = default;

		cLayout_gui& widthFit( float _min, float _max );
		cLayout_gui& widthFit( float _min_max );
		cLayout_gui& widthGrow( float _min, float _max );
		cLayout_gui& widthGrow( float _min_max );
		cLayout_gui& widthFixed( float _width );
		cLayout_gui& widthPercent( float _percent );

		cLayout_gui& heightFit( float _min, float _max );
		cLayout_gui& heightFit( float _min_max );
		cLayout_gui& heightGrow( float _min, float _max );
		cLayout_gui& heightGrow( float _min_max );
		cLayout_gui& heightFixed( float _height );
		cLayout_gui& heightPercent( float _percent );

		cLayout_gui& padding( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom );
		cLayout_gui& padding( std::uint16_t _left_right, std::uint16_t _top_bottom );
		cLayout_gui& padding( std::uint16_t _padding );

		cLayout_gui& margin( std::uint16_t _margin );

		cLayout_gui& horizontalAlignment( eHorizontalAlignment _alignment );
		cLayout_gui& verticalAlignment( eVerticalAlignment _alignment );
		cLayout_gui& alignment( eHorizontalAlignment _horizontal, eVerticalAlignment _vertical );

		cLayout_gui& direction( eDirection _direction );

		const Clay_LayoutConfig& get() const { return m_data; }

	private:
		Clay_LayoutConfig m_data;
	};
}
