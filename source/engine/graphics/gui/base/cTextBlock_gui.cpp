#include "cTextBlock_gui.h"

#include "engine/managers/cFontManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cTextBlockStyle_gui::cTextBlockStyle_gui()
	{
		DF_ProfilingScopeCpu;

		m_data.direction( cWidget_gui::kTopToBottom );
		m_data.alignment( cWidget_gui::kLeftH, cWidget_gui::kTopV );
		m_data.widthFit();
		m_data.heightFit();
	}

	cTextBlock_gui::cTextBlock_gui()
		: m_text_data{}
	{
		DF_ProfilingScopeCpu;

		m_style = MakeShared< cTextBlockStyle_gui >();
	}

	cShared< cTextBlock_gui > cTextBlock_gui::text( const std::string& _text, const bool _store_in_this )
	{
		DF_ProfilingScopeCpu;

		m_text_data.text( _text, _store_in_this );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_text_data.color( _color );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::font( const cFont& _font )
	{
		DF_ProfilingScopeCpu;

		m_text_data.font( _font );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::size( const std::uint16_t _size )
	{
		DF_ProfilingScopeCpu;

		m_text_data.size( _size );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::letterSpacing( const std::uint16_t _letter_spacing )
	{
		DF_ProfilingScopeCpu;

		m_text_data.letterSpacing( _letter_spacing );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::lineHeight( const std::uint16_t _line_height )
	{
		DF_ProfilingScopeCpu;

		m_text_data.lineHeight( _line_height );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::wrapMode( const cText_gui::eWrapMode _wrap_mode )
	{
		DF_ProfilingScopeCpu;

		m_text_data.wrapMode( _wrap_mode );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::textAlignment( const cText_gui::eAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		m_text_data.alignment( _alignment );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::setStyle( const cShared< cTextBlockStyle_gui >& _style )
	{
		DF_ProfilingScopeCpu;

		m_style = _style;

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	void cTextBlock_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		CLAY( m_style->getData() )
		{
			if( !m_text_data.isEmpty() )
				m_text_data.paint();
		}
	}

	void cTextBlock_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		m_text_data.wrapMode( cText_gui::kNewlines );
		m_text_data.alignment( cText_gui::kLeft );
		m_text_data.font( cFontManager::get( "roboto" ) );
		m_text_data.size( 32 );
		m_text_data.color( color::white );
	}
}