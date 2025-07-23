#pragma once

#include "engine/graphics/gui/core/cWidget_gui.h"
#include "engine/graphics/gui/core/iMouseEvents_gui.h"
#include "engine/graphics/gui/core/iWidget_gui.h"

namespace df::gui
{
	class cButton_gui : public iWidgetCreate_gui< cButton_gui >,
						public iMouseEvents_gui
	{
	public:
		DF_DefaultCopyAndMove( cButton_gui );

		cButton_gui()           = default;
		~cButton_gui() override = default;

		cShared< cButton_gui > widthFit( float _min, float _max );
		cShared< cButton_gui > widthFit( float _min_max );
		cShared< cButton_gui > widthFit();
		cShared< cButton_gui > widthGrow( float _min, float _max );
		cShared< cButton_gui > widthGrow( float _min_max );
		cShared< cButton_gui > widthGrow();
		cShared< cButton_gui > widthFixed( float _width );
		cShared< cButton_gui > widthPercent( float _percent );

		cShared< cButton_gui > heightFit( float _min, float _max );
		cShared< cButton_gui > heightFit( float _min_max );
		cShared< cButton_gui > heightFit();
		cShared< cButton_gui > heightGrow( float _min, float _max );
		cShared< cButton_gui > heightGrow( float _min_max );
		cShared< cButton_gui > heightGrow();
		cShared< cButton_gui > heightFixed( float _height );
		cShared< cButton_gui > heightPercent( float _percent );

		cShared< cButton_gui > padding( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom );
		cShared< cButton_gui > padding( std::uint16_t _left_right, std::uint16_t _top_bottom );
		cShared< cButton_gui > padding( std::uint16_t _padding );

		cShared< cButton_gui > margin( std::uint16_t _margin );

		cShared< cButton_gui > direction( cWidget_gui::eDirection _direction );

		cShared< cButton_gui > color( const cColor& _color );

		cShared< cButton_gui > cornerRadius( float _top_left, float _top_right, float _bottom_left, float _bottom_right );
		cShared< cButton_gui > cornerRadius( float _top, float _bottom );
		cShared< cButton_gui > cornerRadius( float _radius );

		cShared< cButton_gui > borderColor( const cColor& _color );
		cShared< cButton_gui > borderWidth( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom, std::uint16_t _between_children );
		cShared< cButton_gui > borderWidth( std::uint16_t _left_right, std::uint16_t _top_bottom, std::uint16_t _between_children );
		cShared< cButton_gui > borderWidth( std::uint16_t _width, std::uint16_t _between_children );

		cShared< cButton_gui > setContent( const cShared< iWidget_gui >& _widget );

		void paint() const override;

	private:
		void initialize() override;

		cShared< iWidget_gui > m_content;

		cWidget_gui m_data;
	};
}
