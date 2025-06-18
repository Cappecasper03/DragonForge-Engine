#pragma once

#include "core/cWidget_gui.h"
#include "core/iWidget_gui.h"
#include "engine/core/utils/cColor.h"

namespace df::gui
{
	template< typename T >
	class iList_gui : public iWidgetCreate_gui< T >
	{
	public:
		DF_DefaultCopyAndMove( iList_gui );

		iList_gui()           = default;
		~iList_gui() override = default;

		cShared< T > color( const cColor& _color );

		cShared< T > widthFit( float _min, float _max );
		cShared< T > widthFit( float _min_max );
		cShared< T > widthGrow( float _min, float _max );
		cShared< T > widthGrow( float _min_max = 0 );
		cShared< T > widthFixed( float _width );
		cShared< T > widthPercent( float _percent );

		cShared< T > heightFit( float _min, float _max );
		cShared< T > heightFit( float _min_max );
		cShared< T > heightGrow( float _min, float _max );
		cShared< T > heightGrow( float _min_max = 0 );
		cShared< T > heightFixed( float _height );
		cShared< T > heightPercent( float _percent );

		cShared< T > padding( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom );
		cShared< T > padding( std::uint16_t _left_right, std::uint16_t _top_bottom );
		cShared< T > padding( std::uint16_t _padding );

		cShared< T > margin( std::uint16_t _margin );

		cShared< T > borderColor( const cColor& _color );
		cShared< T > borderWidth( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom, std::uint16_t _between_children );
		cShared< T > borderWidth( std::uint16_t _left_right, std::uint16_t _top_bottom, std::uint16_t _between_children );
		cShared< T > borderWidth( std::uint16_t _width, std::uint16_t _between_children );

		cShared< T > addSlot( const cShared< iWidget_gui >& _widget );

		void paint() const { m_data.paint(); }

	protected:
		cWidget_gui m_data;
	};

}

#include "iList_gui.inl"