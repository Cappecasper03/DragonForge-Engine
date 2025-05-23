#pragma once

#include <clay.h>
#include <cstdint>

#include "engine/core/utils/Misc.h"

namespace df::gui
{
	class cLayout_gui
	{
	public:
		DF_DeleteCopyAndMove( cLayout_gui );

		enum eSizeType : std::uint8_t
		{
			kFit,
			kGrow,
			kFixed,
			kPercent,
		};

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

		cLayout_gui& width( float _min, float _max, eSizeType _type );
		cLayout_gui& width( float _min_max, eSizeType _type );

		cLayout_gui& height( float _min, float _max, eSizeType _type );
		cLayout_gui& height( float _min_max, eSizeType _type );

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
