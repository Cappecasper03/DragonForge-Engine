#include "cButton_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cButtonStyle_gui::cButtonStyle_gui()
	{
		DF_ProfilingScopeCpu;

		m_data.direction( cWidget_gui::kTopToBottom );
		m_data.alignment( cWidget_gui::kCenterH, cWidget_gui::kCenterV );
		m_data.widthPercent( .5f );
		m_data.heightGrow();

		m_normal  = { .image = nullptr, .image_size = 0, .color = color::gray, .padding = 0, .border_color = color::black, .border_width = 1, .corner_radius = .05f };
		m_hovered = { .image = nullptr, .image_size = 0, .color = color::light_gray, .padding = 0, .border_color = color::black, .border_width = 1, .corner_radius = .05f };
		m_pressed = { .image = nullptr, .image_size = 0, .color = color::dark_gray, .padding = 0, .border_color = color::black, .border_width = 1, .corner_radius = .05f };

		applyBrush( m_normal );
	}

	cShared< cButtonStyle_gui > cButtonStyle_gui::setNormal( const sWidgetBrush& _brush )
	{
		DF_ProfilingScopeCpu;

		m_normal = _brush;

		return std::static_pointer_cast< cButtonStyle_gui >( shared_from_this() );
	}

	cShared< cButtonStyle_gui > cButtonStyle_gui::setHovered( const sWidgetBrush& _brush )
	{
		DF_ProfilingScopeCpu;

		m_hovered = _brush;

		return std::static_pointer_cast< cButtonStyle_gui >( shared_from_this() );
	}

	cShared< cButtonStyle_gui > cButtonStyle_gui::setPressed( const sWidgetBrush& _brush )
	{
		DF_ProfilingScopeCpu;

		m_pressed = _brush;

		return std::static_pointer_cast< cButtonStyle_gui >( shared_from_this() );
	}

	void cButtonStyle_gui::update( const cShared< iWidget_gui >& _widget )
	{
		DF_ProfilingScopeCpu;

		if( _widget->isMouseInside() )
		{
			if( _widget->isMouseButtonDown() )
				applyBrush( m_pressed );
			else
				applyBrush( m_hovered );
		}
		else
			applyBrush( m_normal );
	}

	cButton_gui::cButton_gui()
	{
		DF_ProfilingScopeCpu;

		m_style = MakeShared< cButtonStyle_gui >();
	}

	cShared< cButton_gui > cButton_gui::setStyle( const cShared< cButtonStyle_gui >& _style )
	{
		DF_ProfilingScopeCpu;

		m_style = _style;

		return std::static_pointer_cast< cButton_gui >( shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::setContent( const cShared< iWidget_gui >& _widget )
	{
		DF_ProfilingScopeCpu;

		m_content = _widget;

		return std::static_pointer_cast< cButton_gui >( shared_from_this() );
	}

	void cButton_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		CLAY( m_style->getData() )
		{
			checkHover();

			if( m_content )
				m_content->paint();
		}
	}
}
