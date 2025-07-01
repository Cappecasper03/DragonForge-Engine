#include "cTextBlock_gui.h"

#include "engine/managers/cFontManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cShared< cTextBlock_gui > cTextBlock_gui::text( const std::string& _text, const bool _static_storage )
	{
		DF_ProfilingScopeCpu;

		m_text_data.text( _text, _static_storage );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::textColor( const cColor& _color )
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

	cShared< cTextBlock_gui > cTextBlock_gui::widthFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit( _min, _max );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::widthFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit( _min_max );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::widthFit()
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit();

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::widthGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow( _min, _max );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::widthGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow( _min_max );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::widthGrow()
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow();

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::widthFixed( const float _width )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFixed( _width );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::widthPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.widthPercent( _percent );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::heightFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit( _min, _max );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::heightFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit( _min_max );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::heightFit()
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit();

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::heightGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow( _min, _max );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::heightGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow( _min_max );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::heightGrow()
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow();

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::heightFixed( const float _height )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFixed( _height );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::heightPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.heightPercent( _percent );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::padding( const std::uint16_t _left, const std::uint16_t _right, const std::uint16_t _top, const std::uint16_t _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _left, _right, _top, _bottom );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::padding( const std::uint16_t _left_right, const std::uint16_t _top_bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _left_right, _top_bottom );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::padding( const std::uint16_t _padding )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _padding );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::margin( const std::uint16_t _margin )
	{
		DF_ProfilingScopeCpu;

		m_data.margin( _margin );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::horizontalAlignment( const cWidget_gui::eHorizontalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		m_data.horizontalAlignment( _alignment );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::verticalAlignment( const cWidget_gui::eVerticalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		m_data.verticalAlignment( _alignment );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::alignment( const cWidget_gui::eHorizontalAlignment _horizontal, const cWidget_gui::eVerticalAlignment _vertical )
	{
		DF_ProfilingScopeCpu;

		m_data.alignment( _horizontal, _vertical );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	cShared< cTextBlock_gui > cTextBlock_gui::backgroundColor( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.color( _color );

		return std::static_pointer_cast< cTextBlock_gui >( shared_from_this() );
	}

	void cTextBlock_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		CLAY( m_data.get() )
		{
			if( !m_text_data.isEmpty() )
				m_text_data.paint();
		}
	}

	void cTextBlock_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit();
		m_data.heightFit();
		m_data.alignment( cWidget_gui::kCenterH, cWidget_gui::kCenterV );

		m_text_data.wrapMode( cText_gui::kNewlines );
		m_text_data.alignment( cText_gui::kLeft );
		m_text_data.font( cFontManager::get( "roboto" ) );
		m_text_data.size( 26 );
	}
}