#include "cText_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	std::vector< std::string > cText_gui::m_stored_texts;

	cText_gui::cText_gui()
		: m_text{}
		, m_data{}
	{
		DF_ProfilingScopeCpu;

		m_data.textColor.r = 1;
		m_data.textColor.g = 1;
		m_data.textColor.b = 1;
		m_data.textColor.a = 1;

		wrapMode( kNewlines );
	}

	cText_gui::cText_gui( const std::string& _text )
		: m_data{}
	{
		DF_ProfilingScopeCpu;

		m_data.textColor.r = 1;
		m_data.textColor.g = 1;
		m_data.textColor.b = 1;
		m_data.textColor.a = 1;

		text( _text );
		wrapMode( kNewlines );
	}

	cText_gui& cText_gui::text( const std::string& _text )
	{
		DF_ProfilingScopeCpu;

		m_stored_texts.push_back( _text );

		m_text = {
			.length = static_cast< int >( m_stored_texts.back().size() ),
			.chars  = m_stored_texts.back().data(),
		};

		return *this;
	}

	cText_gui& cText_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.textColor.r = _color.r;
		m_data.textColor.g = _color.g;
		m_data.textColor.b = _color.b;
		m_data.textColor.a = _color.a;

		return *this;
	}

	cText_gui& cText_gui::font( const cFont& _font )
	{
		DF_ProfilingScopeCpu;

		m_data.fontId = 1;

		return *this;
	}

	cText_gui& cText_gui::size( const std::uint16_t _size )
	{
		DF_ProfilingScopeCpu;

		m_data.fontSize = _size;

		return *this;
	}

	cText_gui& cText_gui::letterSpacing( const std::uint16_t _letter_spacing )
	{
		DF_ProfilingScopeCpu;

		m_data.letterSpacing = _letter_spacing;

		return *this;
	}

	cText_gui& cText_gui::lineHeight( const std::uint16_t _line_height )
	{
		DF_ProfilingScopeCpu;

		m_data.lineHeight = _line_height;

		return *this;
	}

	cText_gui& cText_gui::wrapMode( const eWrapMode _wrap_mode )
	{
		DF_ProfilingScopeCpu;

		switch( _wrap_mode )
		{
			case kWords:    m_data.wrapMode = CLAY_TEXT_WRAP_WORDS; break;
			case kNewlines: m_data.wrapMode = CLAY_TEXT_WRAP_NEWLINES; break;
			case kNone:     m_data.wrapMode = CLAY_TEXT_WRAP_NONE; break;
		}

		return *this;
	}

	cText_gui& cText_gui::alignment( const eAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		switch( _alignment )
		{
			case kLeft:   m_data.textAlignment = CLAY_TEXT_ALIGN_LEFT; break;
			case kCenter: m_data.textAlignment = CLAY_TEXT_ALIGN_CENTER; break;
			case kRight:  m_data.textAlignment = CLAY_TEXT_ALIGN_RIGHT; break;
		}

		return *this;
	}

	void cText_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		CLAY_TEXT( m_text, CLAY_TEXT_CONFIG( m_data ) );
	}
}