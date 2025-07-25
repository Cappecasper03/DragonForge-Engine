#pragma once

#include "engine/graphics/gui/core/cText_gui.h"
#include "engine/graphics/gui/core/cWidget_gui.h"
#include "engine/graphics/gui/core/iWidget_gui.h"

namespace df::gui
{
	class cTextBlockStyle_gui final : public iWidgetStyle_gui
	{
	public:
		DF_DefaultCopyAndMove( cTextBlockStyle_gui );

		cTextBlockStyle_gui();
		~cTextBlockStyle_gui() override = default;
	};

	class cTextBlock_gui : public iWidgetCreate_gui< cTextBlock_gui >
	{
	public:
		DF_DefaultCopyAndMove( cTextBlock_gui );

		cTextBlock_gui();
		~cTextBlock_gui() override = default;

		cShared< cTextBlock_gui > text( const std::string& _text, bool _store_in_this = true );
		cShared< cTextBlock_gui > color( const cColor& _color );

		cShared< cTextBlock_gui > font( const cFont& _font );
		cShared< cTextBlock_gui > size( std::uint16_t _size );
		cShared< cTextBlock_gui > letterSpacing( std::uint16_t _letter_spacing );
		cShared< cTextBlock_gui > lineHeight( std::uint16_t _line_height );

		cShared< cTextBlock_gui > wrapMode( cText_gui::eWrapMode _wrap_mode );
		cShared< cTextBlock_gui > textAlignment( cText_gui::eAlignment _alignment );

		cShared< cTextBlock_gui > setStyle( const cShared< cTextBlockStyle_gui >& _style );

		void paint() const override;

	private:
		void initialize() override;

		cText_gui m_text_data;
	};
}
