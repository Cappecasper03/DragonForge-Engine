#pragma once

#include "engine/graphics/gui/core/cText_gui.h"
#include "engine/graphics/gui/core/cWidget_gui.h"
#include "engine/graphics/gui/core/iWidget_gui.h"

namespace df::gui
{
	class cTextBlock_gui : public iWidgetCreate_gui< cTextBlock_gui >
	{
	public:
		DF_DefaultCopyAndMove( cTextBlock_gui );

		cTextBlock_gui()           = default;
		~cTextBlock_gui() override = default;

		cShared< cTextBlock_gui > text( const std::string& _text, bool _static_storage = true );

		cShared< cTextBlock_gui > textColor( const cColor& _color );

		cShared< cTextBlock_gui > font( const cFont& _font );

		cShared< cTextBlock_gui > size( std::uint16_t _size );

		cShared< cTextBlock_gui > letterSpacing( std::uint16_t _letter_spacing );

		cShared< cTextBlock_gui > lineHeight( std::uint16_t _line_height );

		cShared< cTextBlock_gui > wrapMode( cText_gui::eWrapMode _wrap_mode );

		cShared< cTextBlock_gui > textAlignment( cText_gui::eAlignment _alignment );

		cShared< cTextBlock_gui > widthFit( float _min, float _max );
		cShared< cTextBlock_gui > widthFit( float _min_max );
		cShared< cTextBlock_gui > widthFit();
		cShared< cTextBlock_gui > widthGrow( float _min, float _max );
		cShared< cTextBlock_gui > widthGrow( float _min_max );
		cShared< cTextBlock_gui > widthGrow();
		cShared< cTextBlock_gui > widthFixed( float _width );
		cShared< cTextBlock_gui > widthPercent( float _percent );

		cShared< cTextBlock_gui > heightFit( float _min, float _max );
		cShared< cTextBlock_gui > heightFit( float _min_max );
		cShared< cTextBlock_gui > heightFit();
		cShared< cTextBlock_gui > heightGrow( float _min, float _max );
		cShared< cTextBlock_gui > heightGrow( float _min_max );
		cShared< cTextBlock_gui > heightGrow();
		cShared< cTextBlock_gui > heightFixed( float _height );
		cShared< cTextBlock_gui > heightPercent( float _percent );

		cShared< cTextBlock_gui > padding( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom );
		cShared< cTextBlock_gui > padding( std::uint16_t _left_right, std::uint16_t _top_bottom );
		cShared< cTextBlock_gui > padding( std::uint16_t _padding );

		cShared< cTextBlock_gui > margin( std::uint16_t _margin );

		cShared< cTextBlock_gui > alignment( cWidget_gui::eHorizontalAlignment _alignment );
		cShared< cTextBlock_gui > alignment( cWidget_gui::eVerticalAlignment _alignment );
		cShared< cTextBlock_gui > alignment( cWidget_gui::eHorizontalAlignment _horizontal, cWidget_gui::eVerticalAlignment _vertical );

		cShared< cTextBlock_gui > backgroundColor( const cColor& _color );

		void paint() const override;

	private:
		void initialize() override;

		cWidget_gui m_data;
		cText_gui   m_text_data;
	};
}
