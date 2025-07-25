#pragma once

#include <clay.h>
#include <cstdint>
#include <string>

#include "engine/core/utils/cColor.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/cFont.h"

namespace df::gui
{
	class cText_gui
	{
	public:
		DF_DefaultCopyAndMove( cText_gui );

		enum eWrapMode : std::uint8_t
		{
			kWords,
			kNewlines,
			kNone,
		};

		enum eAlignment : std::uint8_t
		{
			kLeft,
			kCenter,
			kRight,
		};

		cText_gui()  = default;
		~cText_gui() = default;

		void text( const std::string& _text, bool _store_in_this = true );
		void color( const cColor& _color );

		void font( const cFont& _font );
		void size( std::uint16_t _size );
		void letterSpacing( std::uint16_t _letter_spacing );
		void lineHeight( std::uint16_t _line_height );

		void wrapMode( eWrapMode _wrap_mode );
		void alignment( eAlignment _alignment );

		void paint() const;

		bool isEmpty() const { return !m_text.length; }

		const Clay_TextElementConfig& get() const { return m_data; }

		static void clearStoredTexts() { m_stored_texts.clear(); }

	private:
		Clay_String            m_text;
		Clay_TextElementConfig m_data;

		std::string                       m_stored_text;
		static std::vector< std::string > m_stored_texts;
	};
}
