#include "cButton_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cShared< cButton_gui > cButton_gui::widthFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit( _min, _max );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::widthFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit( _min_max );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::widthFit()
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit();

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::widthGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow( _min, _max );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::widthGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow( _min_max );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::widthGrow()
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow();

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::widthFixed( const float _width )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFixed( _width );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::widthPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.widthPercent( _percent );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::heightFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit( _min, _max );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::heightFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit( _min_max );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::heightFit()
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit();

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::heightGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow( _min, _max );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::heightGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow( _min_max );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::heightGrow()
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow();

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::heightFixed( const float _height )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFixed( _height );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::heightPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.heightPercent( _percent );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::padding( const std::uint16_t _left, const std::uint16_t _right, const std::uint16_t _top, const std::uint16_t _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _left, _right, _top, _bottom );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::padding( const std::uint16_t _left_right, const std::uint16_t _top_bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _left_right, _top_bottom );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::padding( const std::uint16_t _padding )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _padding );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::margin( const std::uint16_t _margin )
	{
		DF_ProfilingScopeCpu;

		m_data.margin( _margin );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::direction( const cWidget_gui::eDirection _direction )
	{
		DF_ProfilingScopeCpu;

		m_data.direction( _direction );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.color( _color );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::cornerRadius( const float _top_left, const float _top_right, const float _bottom_left, const float _bottom_right )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius( _top_left, _top_right, _bottom_left, _bottom_right );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::cornerRadius( const float _top, const float _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius( _top, _bottom );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::cornerRadius( const float _radius )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius( _radius );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::borderColor( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.borderColor( _color );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::borderWidth( const std::uint16_t _left,
	                                                 const std::uint16_t _right,
	                                                 const std::uint16_t _top,
	                                                 const std::uint16_t _bottom,
	                                                 const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.borderWidth( _left, _right, _top, _bottom, _between_children );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::borderWidth( const std::uint16_t _left_right, const std::uint16_t _top_bottom, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.borderWidth( _left_right, _top_bottom, _between_children );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::borderWidth( const std::uint16_t _width, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.borderWidth( _width, _between_children );

		return std::static_pointer_cast< cButton_gui >( this->shared_from_this() );
	}

	cShared< cButton_gui > cButton_gui::setContent( const cShared< iWidget_gui >& _widget )
	{
		DF_ProfilingScopeCpu;

		m_content = _widget;

		return std::static_pointer_cast< cButton_gui >( shared_from_this() );
	}

	void cButton_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		CLAY( m_data.get() )
		{
			checkHover();

			if( m_content )
				m_content->paint();
		}
	}

	void cButton_gui::initialize()
	{
		DF_ProfilingScopeCpu;

		m_data.direction( cWidget_gui::kTopToBottom );
		m_data.alignment( cWidget_gui::kCenterH, cWidget_gui::kCenterV );
		m_data.widthGrow();
		m_data.heightGrow();
	}
}
