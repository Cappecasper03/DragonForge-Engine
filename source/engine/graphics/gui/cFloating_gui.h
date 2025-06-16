#pragma once

#include <clay.h>

#include "engine/core/math/cVector.h"
#include "engine/core/utils/Misc.h"

namespace df::gui
{
	class cWidget_gui;

	class cFloating_gui
	{
	public:
		DF_DefaultCopyAndMove( cFloating_gui );

		enum eAttachPoint : std::uint8_t
		{
			kLeftTop,
			kLeftCenter,
			kLeftBottom,
			kCenterTop,
			kCenter,
			kCenterBottom,
			kRightTop,
			kRightCenter,
			kRightBottom,
		};

		enum eInputMode : std::uint8_t
		{
			kCapture,
			kPassthrough,
		};

		enum eAttachMode : std::uint8_t
		{
			kNone,
			kParent,
			kElement,
			kRoot,
		};

		cFloating_gui();
		~cFloating_gui() = default;

		cFloating_gui& offset( const cVector2f& _offset );

		cFloating_gui& expand( const cVector2f& _expand );

		cFloating_gui& elementId( const cWidget_gui* _element_id );

		cFloating_gui& depthIndex( std::uint16_t _index );

		cFloating_gui& attachPoints( eAttachPoint _element, eAttachPoint _parent );

		cFloating_gui& inputMode( eInputMode _mode );

		cFloating_gui& attachMode( eAttachMode _mode );

		cFloating_gui& clipToParent( bool _clip_to_parent );

		const Clay_FloatingElementConfig& get() const { return m_data; }

	private:
		Clay_FloatingElementConfig m_data;
	};
}
