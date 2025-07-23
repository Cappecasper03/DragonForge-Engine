#include "cWidget_gui.h"

#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	void cWidget_gui::id( const std::string& _id )
	{
		DF_ProfilingScopeCpu;

		m_data.id = Clay__HashString( Clay_String{ .isStaticallyAllocated = true, .length = static_cast< int >( _id.size() ), .chars = _id.data() }, 0, 0 );
	}

	void cWidget_gui::widthFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.width = CLAY_SIZING_FIT( _min, _max );
	}

	void cWidget_gui::widthFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.width = CLAY_SIZING_FIT( _min_max, _min_max );
	}

	void cWidget_gui::widthFit()
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.width = CLAY_SIZING_FIT( 0, std::numeric_limits< float >::max() );
	}

	void cWidget_gui::widthGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.width = CLAY_SIZING_GROW( _min, _max );
	}

	void cWidget_gui::widthGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.width = CLAY_SIZING_GROW( _min_max, _min_max );
	}

	void cWidget_gui::widthGrow()
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.width = CLAY_SIZING_GROW( 0, std::numeric_limits< float >::max() );
	}

	void cWidget_gui::widthFixed( const float _width )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.width = CLAY_SIZING_FIXED( _width );
	}

	void cWidget_gui::widthPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.width = CLAY_SIZING_PERCENT( _percent );
	}

	void cWidget_gui::heightFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.height = CLAY_SIZING_FIT( _min, _max );
	}

	void cWidget_gui::heightFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.height = CLAY_SIZING_FIT( _min_max, _min_max );
	}

	void cWidget_gui::heightFit()
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.height = CLAY_SIZING_FIT( 0, std::numeric_limits< float >::max() );
	}

	void cWidget_gui::heightGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.height = CLAY_SIZING_GROW( _min, _max );
	}

	void cWidget_gui::heightGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.height = CLAY_SIZING_GROW( _min_max, _min_max );
	}

	void cWidget_gui::heightGrow()
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.height = CLAY_SIZING_GROW( 0, std::numeric_limits< float >::max() );
	}

	void cWidget_gui::heightFixed( const float _height )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.height = CLAY_SIZING_FIXED( _height );
	}

	void cWidget_gui::heightPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.sizing.height = CLAY_SIZING_PERCENT( _percent );
	}

	void cWidget_gui::padding( const std::uint16_t _left, const std::uint16_t _right, const std::uint16_t _top, const std::uint16_t _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.padding.left   = _left;
		m_data.layout.padding.right  = _right;
		m_data.layout.padding.top    = _top;
		m_data.layout.padding.bottom = _bottom;
	}

	void cWidget_gui::padding( const std::uint16_t _left_right, const std::uint16_t _top_bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.padding.left   = _left_right;
		m_data.layout.padding.right  = _left_right;
		m_data.layout.padding.top    = _top_bottom;
		m_data.layout.padding.bottom = _top_bottom;
	}

	void cWidget_gui::padding( const std::uint16_t _padding )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.padding.left   = _padding;
		m_data.layout.padding.right  = _padding;
		m_data.layout.padding.top    = _padding;
		m_data.layout.padding.bottom = _padding;
	}

	void cWidget_gui::margin( const std::uint16_t _margin )
	{
		DF_ProfilingScopeCpu;

		m_data.layout.childGap = _margin;
	}

	void cWidget_gui::alignment( const eHorizontalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		switch( _alignment )
		{
			case kLeftH:   m_data.layout.childAlignment.x = CLAY_ALIGN_X_LEFT; break;
			case kRightH:  m_data.layout.childAlignment.x = CLAY_ALIGN_X_RIGHT; break;
			case kCenterH: m_data.layout.childAlignment.x = CLAY_ALIGN_X_CENTER; break;
		}
	}

	void cWidget_gui::alignment( const eVerticalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		switch( _alignment )
		{
			case kTopV:    m_data.layout.childAlignment.y = CLAY_ALIGN_Y_TOP; break;
			case kBottomV: m_data.layout.childAlignment.y = CLAY_ALIGN_Y_BOTTOM; break;
			case kCenterV: m_data.layout.childAlignment.y = CLAY_ALIGN_Y_CENTER; break;
		}
	}

	void cWidget_gui::alignment( const eHorizontalAlignment _horizontal, const eVerticalAlignment _vertical )
	{
		DF_ProfilingScopeCpu;

		alignment( _horizontal );
		alignment( _vertical );
	}

	void cWidget_gui::direction( const eDirection _direction )
	{
		DF_ProfilingScopeCpu;

		switch( _direction )
		{
			case kLeftToRight: m_data.layout.layoutDirection = CLAY_LEFT_TO_RIGHT; break;
			case kTopToBottom: m_data.layout.layoutDirection = CLAY_TOP_TO_BOTTOM; break;
		}
	}

	void cWidget_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.backgroundColor.r = _color.r;
		m_data.backgroundColor.g = _color.g;
		m_data.backgroundColor.b = _color.b;
		m_data.backgroundColor.a = _color.a;
	}

	void cWidget_gui::cornerRadius( const float _top_left, const float _top_right, const float _bottom_left, const float _bottom_right )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius.topLeft     = _top_left;
		m_data.cornerRadius.topRight    = _top_right;
		m_data.cornerRadius.bottomLeft  = _bottom_left;
		m_data.cornerRadius.bottomRight = _bottom_right;
	}

	void cWidget_gui::cornerRadius( const float _top, const float _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius.topLeft     = _top;
		m_data.cornerRadius.topRight    = _top;
		m_data.cornerRadius.bottomLeft  = _bottom;
		m_data.cornerRadius.bottomRight = _bottom;
	}

	void cWidget_gui::cornerRadius( const float _radius )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius.topLeft     = _radius;
		m_data.cornerRadius.topRight    = _radius;
		m_data.cornerRadius.bottomLeft  = _radius;
		m_data.cornerRadius.bottomRight = _radius;
	}

	void cWidget_gui::image( cTexture2D* _texture )
	{
		DF_ProfilingScopeCpu;

		m_data.image.imageData         = _texture;
		m_data.aspectRatio.aspectRatio = static_cast< float >( _texture->getSize().width() ) / static_cast< float >( _texture->getSize().height() );
	}

	void cWidget_gui::image( cTexture2D* _texture, const cVector2f& _size )
	{
		DF_ProfilingScopeCpu;

		m_data.image.imageData         = _texture;
		m_data.aspectRatio.aspectRatio = _size.width() / _size.height();
	}

	void cWidget_gui::floatingOffset( const cVector2f& _offset )
	{
		DF_ProfilingScopeCpu;

		m_data.floating.offset.x = _offset.x();
		m_data.floating.offset.y = _offset.y();
	}

	void cWidget_gui::floatingExpand( const cVector2f& _expand )
	{
		DF_ProfilingScopeCpu;

		m_data.floating.expand.width  = _expand.width();
		m_data.floating.expand.height = _expand.height();
	}

	void cWidget_gui::floatingElementId( const cWidget_gui* _element_id )
	{
		DF_ProfilingScopeCpu;

		m_data.floating.parentId = _element_id->m_data.id.id;
	}

	void cWidget_gui::floatingDepthIndex( const std::int16_t _index )
	{
		DF_ProfilingScopeCpu;

		m_data.floating.zIndex = _index;
	}

	void cWidget_gui::floatingAttachPoints( const eAttachPoint _element, const eAttachPoint _parent )
	{
		DF_ProfilingScopeCpu;

		switch( _element )
		{
			case kLeftTop:      m_data.floating.attachPoints.element = CLAY_ATTACH_POINT_LEFT_TOP; break;
			case kLeftCenter:   m_data.floating.attachPoints.element = CLAY_ATTACH_POINT_LEFT_CENTER; break;
			case kLeftBottom:   m_data.floating.attachPoints.element = CLAY_ATTACH_POINT_LEFT_BOTTOM; break;
			case kCenterTop:    m_data.floating.attachPoints.element = CLAY_ATTACH_POINT_CENTER_TOP; break;
			case kCenter:       m_data.floating.attachPoints.element = CLAY_ATTACH_POINT_CENTER_CENTER; break;
			case kCenterBottom: m_data.floating.attachPoints.element = CLAY_ATTACH_POINT_CENTER_BOTTOM; break;
			case kRightTop:     m_data.floating.attachPoints.element = CLAY_ATTACH_POINT_RIGHT_TOP; break;
			case kRightCenter:  m_data.floating.attachPoints.element = CLAY_ATTACH_POINT_RIGHT_CENTER; break;
			case kRightBottom:  m_data.floating.attachPoints.element = CLAY_ATTACH_POINT_RIGHT_BOTTOM; break;
		}

		switch( _parent )
		{
			case kLeftTop:      m_data.floating.attachPoints.parent = CLAY_ATTACH_POINT_LEFT_TOP; break;
			case kLeftCenter:   m_data.floating.attachPoints.parent = CLAY_ATTACH_POINT_LEFT_CENTER; break;
			case kLeftBottom:   m_data.floating.attachPoints.parent = CLAY_ATTACH_POINT_LEFT_BOTTOM; break;
			case kCenterTop:    m_data.floating.attachPoints.parent = CLAY_ATTACH_POINT_CENTER_TOP; break;
			case kCenter:       m_data.floating.attachPoints.parent = CLAY_ATTACH_POINT_CENTER_CENTER; break;
			case kCenterBottom: m_data.floating.attachPoints.parent = CLAY_ATTACH_POINT_CENTER_BOTTOM; break;
			case kRightTop:     m_data.floating.attachPoints.parent = CLAY_ATTACH_POINT_RIGHT_TOP; break;
			case kRightCenter:  m_data.floating.attachPoints.parent = CLAY_ATTACH_POINT_RIGHT_CENTER; break;
			case kRightBottom:  m_data.floating.attachPoints.parent = CLAY_ATTACH_POINT_RIGHT_BOTTOM; break;
		}
	}

	void cWidget_gui::floatingInputMode( const eInputMode _mode )
	{
		DF_ProfilingScopeCpu;

		switch( _mode )
		{
			case kCapture:     m_data.floating.pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE; break;
			case kPassthrough: m_data.floating.pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH; break;
		}
	}

	void cWidget_gui::floatingAttachMode( const eAttachMode _mode )
	{
		DF_ProfilingScopeCpu;

		switch( _mode )
		{
			case kNone:    m_data.floating.attachTo = CLAY_ATTACH_TO_NONE; break;
			case kParent:  m_data.floating.attachTo = CLAY_ATTACH_TO_PARENT; break;
			case kElement: m_data.floating.attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID; break;
			case kRoot:    m_data.floating.attachTo = CLAY_ATTACH_TO_ROOT; break;
		}
	}

	void cWidget_gui::floatingClipToParent( const bool _clip_to_parent )
	{
		DF_ProfilingScopeCpu;

		m_data.floating.clipTo = _clip_to_parent ? CLAY_CLIP_TO_ATTACHED_PARENT : CLAY_CLIP_TO_NONE;
	}

	void cWidget_gui::borderColor( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.border.color.r = _color.r;
		m_data.border.color.g = _color.g;
		m_data.border.color.b = _color.b;
		m_data.border.color.a = _color.a;
	}

	void cWidget_gui::borderWidth( const std::uint16_t _left,
	                               const std::uint16_t _right,
	                               const std::uint16_t _top,
	                               const std::uint16_t _bottom,
	                               const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.border.width.left            = _left;
		m_data.border.width.right           = _right;
		m_data.border.width.top             = _top;
		m_data.border.width.bottom          = _bottom;
		m_data.border.width.betweenChildren = _between_children;
	}

	void cWidget_gui::borderWidth( const std::uint16_t _left_right, const std::uint16_t _top_bottom, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.border.width.left            = _left_right;
		m_data.border.width.right           = _left_right;
		m_data.border.width.top             = _top_bottom;
		m_data.border.width.bottom          = _top_bottom;
		m_data.border.width.betweenChildren = _between_children;
	}

	void cWidget_gui::borderWidth( const std::uint16_t _width, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.border.width.left            = _width;
		m_data.border.width.right           = _width;
		m_data.border.width.top             = _width;
		m_data.border.width.bottom          = _width;
		m_data.border.width.betweenChildren = _between_children;
	}
}