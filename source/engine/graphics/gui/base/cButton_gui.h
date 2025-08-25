#pragma once

#include "engine/graphics/gui/core/iWidget_gui.h"
#include "engine/graphics/gui/core/iWidgetStyle_gui.h"

namespace df::gui
{
	class cButtonStyle_gui final : public iWidgetStyle_gui
	{
	public:
		DF_DefaultCopyAndMove( cButtonStyle_gui );

		cButtonStyle_gui();
		~cButtonStyle_gui() override = default;

		cShared< cButtonStyle_gui > setNormal( const sWidgetBrush& _brush );
		cShared< cButtonStyle_gui > setHovered( const sWidgetBrush& _brush );
		cShared< cButtonStyle_gui > setPressed( const sWidgetBrush& _brush );

	private:
		void update( const cShared< iWidget_gui >& _widget ) override;

		sWidgetBrush m_normal;
		sWidgetBrush m_hovered;
		sWidgetBrush m_pressed;
	};

	class cButton_gui : public iWidgetCreate_gui< cButton_gui >
	{
	public:
		DF_DefaultCopyAndMove( cButton_gui );

		cButton_gui();
		~cButton_gui() override = default;

		cShared< cButton_gui > onMouseButtonDown( const std::function< void() >& _function );
		cShared< cButton_gui > onMouseButtonUp( const std::function< void() >& _function );

		cShared< cButton_gui > onMouseEnter( const std::function< void() >& _function );
		cShared< cButton_gui > onMouseLeave( const std::function< void() >& _function );

		cShared< cButton_gui > setStyle( const cShared< cButtonStyle_gui >& _style );

		cShared< cButton_gui > setContent( const cShared< iWidget_gui >& _widget );

		void paint() const override;

	private:
		cShared< iWidget_gui > m_content;
	};
}
