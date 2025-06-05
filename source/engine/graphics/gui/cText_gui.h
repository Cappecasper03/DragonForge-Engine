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

		cText_gui();
		cText_gui( const std::string& _text );
		~cText_gui() = default;

		cText_gui& text( const std::string& _text );

		cText_gui& color( const cColor& _color );

		cText_gui& font( const cFont& _font );

		cText_gui& size( std::uint16_t _size );

		cText_gui& letterSpacing( std::uint16_t _letter_spacing );

		cText_gui& lineHeight( std::uint16_t _line_height );

		cText_gui& wrapMode( eWrapMode _wrap_mode );

		cText_gui& alignment( eAlignment _alignment );

		void paint() const;

		bool isEmpty() const { return !m_text.length; }

		const Clay_TextElementConfig& get() const { return m_data; }

	private:
		Clay_String            m_text;
		Clay_TextElementConfig m_data;
	};
}
