#include "cTextBlock_gui.h"

#include "engine/managers/cFontManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cShared< cTextBlock_gui > cTextBlock_gui::text( const std::string& _text, const bool _static_storage )
	{
		DF_ProfilingScopeCpu;

		m_data.text( _text, _static_storage );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.color( _color );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::font( const cFont& _font )
	{
		DF_ProfilingScopeCpu;

		m_data.font( _font );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::size( const std::uint16_t _size )
	{
		DF_ProfilingScopeCpu;

		m_data.size( _size );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::letterSpacing( const std::uint16_t _letter_spacing )
	{
		DF_ProfilingScopeCpu;

		m_data.letterSpacing( _letter_spacing );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::lineHeight( const std::uint16_t _line_height )
	{
		DF_ProfilingScopeCpu;

		m_data.lineHeight( _line_height );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::wrapMode( const cText_gui::eWrapMode _wrap_mode )
	{
		DF_ProfilingScopeCpu;

		m_data.wrapMode( _wrap_mode );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::alignment( const cText_gui::eAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		m_data.alignment( _alignment );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	void cTextBlock_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		if( !m_data.isEmpty() )
			m_data.paint();
	}

	void cTextBlock_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		m_data.wrapMode( cText_gui::kNewlines );
		m_data.alignment( cText_gui::kLeft );
		m_data.font( cFontManager::get( "roboto" ) );
		m_data.size( 26 );
	}
}