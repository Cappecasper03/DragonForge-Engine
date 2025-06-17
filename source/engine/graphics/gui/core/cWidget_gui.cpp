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

	void cWidget_gui::horizontalAlignment( const eHorizontalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		switch( _alignment )
		{
			case kLeftH:   m_data.layout.childAlignment.x = CLAY_ALIGN_X_LEFT; break;
			case kRightH:  m_data.layout.childAlignment.x = CLAY_ALIGN_X_RIGHT; break;
			case kCenterH: m_data.layout.childAlignment.x = CLAY_ALIGN_X_CENTER; break;
		}
	}

	void cWidget_gui::verticalAlignment( const eVerticalAlignment _alignment )
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

		horizontalAlignment( _horizontal );
		verticalAlignment( _vertical );
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

	void cWidget_gui::floating( const cFloating_gui& _floating )
	{
		DF_ProfilingScopeCpu;

		m_data.floating = _floating.get();
	}

	void cWidget_gui::border( const cBorder_gui& _border )
	{
		DF_ProfilingScopeCpu;

		m_data.border = _border.get();
	}

	void cWidget_gui::addSlot( const cShared< iWidget_gui >& _widget )
	{
		DF_ProfilingScopeCpu;

		m_slots.push_back( _widget );
	}

	void cWidget_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		CLAY( m_data )
		{
			for( const cShared< iWidget_gui >& slot: m_slots )
				slot->paint();
		}
	}
}