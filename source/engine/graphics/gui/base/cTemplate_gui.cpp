#include "cTemplate_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cShared< cTemplate_gui > cTemplate_gui::id( const std::string& _id )
	{
		DF_ProfilingScopeCpu;

		m_data.id( _id );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::widthFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit( _min, _max );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::widthFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit( _min_max );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::widthFit()
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit();

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::widthGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow( _min, _max );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::widthGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow( _min_max );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::widthGrow()
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow();

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::widthFixed( const float _width )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFixed( _width );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::widthPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.widthPercent( _percent );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::heightFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit( _min, _max );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::heightFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit( _min_max );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::heightFit()
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit();

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::heightGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow( _min, _max );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::heightGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow( _min_max );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::heightGrow()
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow();

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::heightFixed( const float _height )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFixed( _height );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::heightPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.heightPercent( _percent );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::padding( const std::uint16_t _left, const std::uint16_t _right, const std::uint16_t _top, const std::uint16_t _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _left, _right, _top, _bottom );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::padding( const std::uint16_t _left_right, const std::uint16_t _top_bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _left_right, _top_bottom );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::padding( const std::uint16_t _padding )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _padding );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::margin( const std::uint16_t _margin )
	{
		DF_ProfilingScopeCpu;

		m_data.margin( _margin );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::alignment( const cWidget_gui::eHorizontalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		m_data.alignment( _alignment );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::alignment( const cWidget_gui::eVerticalAlignment _alignment )
	{
		DF_ProfilingScopeCpu;

		m_data.alignment( _alignment );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::alignment( const cWidget_gui::eHorizontalAlignment _horizontal, const cWidget_gui::eVerticalAlignment _vertical )
	{
		DF_ProfilingScopeCpu;

		m_data.alignment( _horizontal, _vertical );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::direction( const cWidget_gui::eDirection _direction )
	{
		DF_ProfilingScopeCpu;

		m_data.direction( _direction );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.color( _color );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::cornerRadius( const float _top_left, const float _top_right, const float _bottom_left, const float _bottom_right )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius( _top_left, _top_right, _bottom_left, _bottom_right );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::cornerRadius( const float _top, const float _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius( _top, _bottom );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::cornerRadius( const float _radius )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius( _radius );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::image( cTexture2D* _texture )
	{
		DF_ProfilingScopeCpu;

		m_data.image( _texture );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::image( cTexture2D* _texture, const cVector2f& _size )
	{
		DF_ProfilingScopeCpu;

		m_data.image( _texture, _size );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::floatingOffset( const cVector2f& _offset )
	{
		DF_ProfilingScopeCpu;

		m_data.floatingOffset( _offset );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::floatingExpand( const cVector2f& _expand )
	{
		DF_ProfilingScopeCpu;

		m_data.floatingExpand( _expand );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::floatingElementId( const cWidget_gui* _element_id )
	{
		DF_ProfilingScopeCpu;

		m_data.floatingElementId( _element_id );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::floatingDepthIndex( const std::int16_t _index )
	{
		DF_ProfilingScopeCpu;

		m_data.floatingDepthIndex( _index );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::floatingAttachPoints( const cWidget_gui::eAttachPoint _element, const cWidget_gui::eAttachPoint _parent )
	{
		DF_ProfilingScopeCpu;

		m_data.floatingAttachPoints( _element, _parent );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::floatingInputMode( const cWidget_gui::eInputMode _mode )
	{
		DF_ProfilingScopeCpu;

		m_data.floatingInputMode( _mode );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::floatingAttachMode( const cWidget_gui::eAttachMode _mode )
	{
		DF_ProfilingScopeCpu;

		m_data.floatingAttachMode( _mode );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::floatingClipToParent( const bool _clip_to_parent )
	{
		DF_ProfilingScopeCpu;

		m_data.floatingClipToParent( _clip_to_parent );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::borderColor( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.borderColor( _color );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::borderWidth( const std::uint16_t _left,
	                                                     const std::uint16_t _right,
	                                                     const std::uint16_t _top,
	                                                     const std::uint16_t _bottom,
	                                                     const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.borderWidth( _left, _right, _top, _bottom, _between_children );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::borderWidth( const std::uint16_t _left_right, const std::uint16_t _top_bottom, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.borderWidth( _left_right, _top_bottom, _between_children );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	cShared< cTemplate_gui > cTemplate_gui::borderWidth( const std::uint16_t _width, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.borderWidth( _width, _between_children );

		return std::static_pointer_cast< cTemplate_gui >( this->shared_from_this() );
	}

	void cTemplate_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		CLAY( m_data.get() ) {}
	}

	void cTemplate_gui::initialize() { DF_ProfilingScopeCpu; }
}
