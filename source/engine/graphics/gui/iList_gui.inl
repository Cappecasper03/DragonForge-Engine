#pragma once

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	template< typename T >
	cShared< T > iList_gui< T >::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.color( _color );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::widthFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit( _min, _max );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::widthFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFit( _min_max );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::widthGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow( _min, _max );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::widthGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.widthGrow( _min_max );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::widthFixed( const float _width )
	{
		DF_ProfilingScopeCpu;

		m_data.widthFixed( _width );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::widthPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.widthPercent( _percent );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::heightFit( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit( _min, _max );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::heightFit( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFit( _min_max );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::heightGrow( const float _min, const float _max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow( _min, _max );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::heightGrow( const float _min_max )
	{
		DF_ProfilingScopeCpu;

		m_data.heightGrow( _min_max );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::heightFixed( const float _height )
	{
		DF_ProfilingScopeCpu;

		m_data.heightFixed( _height );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::heightPercent( const float _percent )
	{
		DF_ProfilingScopeCpu;

		m_data.heightPercent( _percent );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::padding( const std::uint16_t _left, const std::uint16_t _right, const std::uint16_t _top, const std::uint16_t _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _left, _right, _top, _bottom );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::padding( const std::uint16_t _left_right, const std::uint16_t _top_bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _left_right, _top_bottom );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::padding( const std::uint16_t _padding )
	{
		DF_ProfilingScopeCpu;

		m_data.padding( _padding );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::margin( const std::uint16_t _margin )
	{
		DF_ProfilingScopeCpu;

		m_data.margin( _margin );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::borderColor( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.borderColor( _color );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::borderWidth( const std::uint16_t _left,
	                                          const std::uint16_t _right,
	                                          const std::uint16_t _top,
	                                          const std::uint16_t _bottom,
	                                          const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.borderWidth( _left, _right, _top, _bottom, _between_children );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::borderWidth( const std::uint16_t _left_right, const std::uint16_t _top_bottom, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.borderWidth( _left_right, _top_bottom, _between_children );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::borderWidth( const std::uint16_t _width, const std::uint16_t _between_children )
	{
		DF_ProfilingScopeCpu;

		m_data.borderWidth( _width, _between_children );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}

	template< typename T >
	cShared< T > iList_gui< T >::addSlot( const cShared< iWidget_gui >& _widget )
	{
		DF_ProfilingScopeCpu;

		m_data.addSlot( _widget );

		return std::static_pointer_cast< T >( this->shared_from_this() );
	}
}