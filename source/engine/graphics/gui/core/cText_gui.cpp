#include "cText_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	std::vector< std::string > cText_gui::m_stored_texts;

	void cText_gui::text( const std::string& _text, const bool _static_storage )
	{
		DF_ProfilingScopeCpu;

		if( _static_storage )
		{
			m_stored_text = _text;

			m_text = {
				.isStaticallyAllocated = true,
				.length                = static_cast< int >( m_stored_text.size() ),
				.chars                 = m_stored_text.data(),
			};
		}
		else
		{
			m_stored_texts.push_back( _text );

			m_text = {
				.isStaticallyAllocated = true,
				.length                = static_cast< int >( m_stored_texts.back().size() ),
				.chars                 = m_stored_texts.back().data(),
			};
		}
	}

	void cText_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.textColor.r = _color.r;
		m_data.textColor.g = _color.g;
		m_data.textColor.b = _color.b;
		m_data.textColor.a = _color.a;
	}

	void cText_gui::font( const cFont& _font )
	{
		DF_ProfilingScopeCpu;

		m_data.fontId = _font.getId();
	}

	void cText_gui::size( const std::uint16_t _size )
	{
		DF_ProfilingScopeCpu;

		m_data.fontSize = _size;
	}

	void cText_gui::letterSpacing( const std::uint16_t _letter_spacing )
	{
		DF_ProfilingScopeCpu;

		m_data.letterSpacing = _letter_spacing;
	}

	void cText_gui::lineHeight( const std::uint16_t _line_height )
	{
		DF_ProfilingScopeCpu;

		m_data.lineHeight = _line_height;
	}

	void cText_gui::wrapMode( const eWrapMode _wrap_mode )
	{
		DF_ProfilingScopeCpu;

		switch( _wrap_mode )
		{
			case kWords:    m_data.wrapMode = CLAY_TEXT_WRAP_WORDS; break;
			case kNewlines: m_data.wrapMode = CLAY_TEXT_WRAP_NEWLINES; break;
			case kNone:     m_data.wrapMode = CLAY_TEXT_WRAP_NONE; break;
		}
	}

	void cText_gui::alignment( const eAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		switch( _alignment )
		{
			case kLeft:   m_data.textAlignment = CLAY_TEXT_ALIGN_LEFT; break;
			case kCenter: m_data.textAlignment = CLAY_TEXT_ALIGN_CENTER; break;
			case kRight:  m_data.textAlignment = CLAY_TEXT_ALIGN_RIGHT; break;
		}
	}

	void cText_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		CLAY_TEXT( m_text, CLAY_TEXT_CONFIG( m_data ) );
	}
}