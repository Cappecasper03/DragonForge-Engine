#pragma once

#include <string>
#include <vector>

#include "cBorder_gui.h"
#include "cFloating_gui.h"
#include "engine/core/math/cVector.h"
#include "engine/core/utils/cColor.h"
#include "engine/core/utils/Misc.h"
#include "iWidget_gui.h"

namespace df
{
	class cTexture2D;
}

namespace df::gui
{
	class cWidget_gui
	{
		friend cFloating_gui;

	public:
		DF_DefaultCopyAndMove( cWidget_gui );

		enum eHorizontalAlignment : std::uint8_t
		{
			kLeftH,
			kRightH,
			kCenterH,
		};

		enum eVerticalAlignment : std::uint8_t
		{
			kTopV,
			kBottomV,
			kCenterV,
		};

		enum eDirection : std::uint8_t
		{
			kLeftToRight,
			kTopToBottom,
		};

		cWidget_gui()  = default;
		~cWidget_gui() = default;

		void id( const std::string& _id );

		void widthFit( float _min, float _max );
		void widthFit( float _min_max );
		void widthGrow( float _min, float _max );
		void widthGrow( float _min_max = 0 );
		void widthFixed( float _width );
		void widthPercent( float _percent );

		void heightFit( float _min, float _max );
		void heightFit( float _min_max );
		void heightGrow( float _min, float _max );
		void heightGrow( float _min_max = 0 );
		void heightFixed( float _height );
		void heightPercent( float _percent );

		void padding( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom );
		void padding( std::uint16_t _left_right, std::uint16_t _top_bottom );
		void padding( std::uint16_t _padding );

		void margin( std::uint16_t _margin );

		void horizontalAlignment( eHorizontalAlignment _alignment );
		void verticalAlignment( eVerticalAlignment _alignment );
		void alignment( eHorizontalAlignment _horizontal, eVerticalAlignment _vertical );

		void direction( eDirection _direction );

		void color( const cColor& _color );

		void cornerRadius( float _top_left, float _top_right, float _bottom_left, float _bottom_right );
		void cornerRadius( float _top, float _bottom );
		void cornerRadius( float _radius );

		void image( cTexture2D* _texture );
		void image( cTexture2D* _texture, const cVector2f& _size );

		void floating( const cFloating_gui& _floating );

		void border( const cBorder_gui& _border );

		void addSlot( const cShared< iWidget_gui >& _widget );

		void paint() const;

	protected:
		Clay_ElementDeclaration m_data;

		std::vector< cShared< iWidget_gui > > m_slots;
	};
}
