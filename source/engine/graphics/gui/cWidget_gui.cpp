﻿#include "cWidget_gui.h"

#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cWidget_gui::cWidget_gui()
		: m_data{}
	{
		DF_ProfilingScopeCpu;

		m_data.backgroundColor.r = 1;
		m_data.backgroundColor.g = 1;
		m_data.backgroundColor.b = 1;
		m_data.backgroundColor.a = 1;
	}

	cWidget_gui::cWidget_gui( const std::string& _id )
		: m_data{}
	{
		DF_ProfilingScopeCpu;

		m_data.backgroundColor.r = 1;
		m_data.backgroundColor.g = 1;
		m_data.backgroundColor.b = 1;
		m_data.backgroundColor.a = 1;

		id( _id );
	}

	cWidget_gui& cWidget_gui::id( const std::string& _id )
	{
		DF_ProfilingScopeCpu;

		m_data.id = Clay__HashString( Clay_String{ .length = static_cast< int >( _id.size() ), .chars = _id.data() }, 0, 0 );

		return *this;
	}

	cWidget_gui& cWidget_gui::layout( const cLayout_gui& _layout )
	{
		DF_ProfilingScopeCpu;

		m_data.layout = _layout.get();

		return *this;
	}

	cWidget_gui& cWidget_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.backgroundColor.r = _color.r;
		m_data.backgroundColor.g = _color.g;
		m_data.backgroundColor.b = _color.b;
		m_data.backgroundColor.a = _color.a;

		return *this;
	}

	cWidget_gui& cWidget_gui::cornerRadius( const float _top_left, const float _top_right, const float _bottom_left, const float _bottom_right )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius.topLeft     = _top_left;
		m_data.cornerRadius.topRight    = _top_right;
		m_data.cornerRadius.bottomLeft  = _bottom_left;
		m_data.cornerRadius.bottomRight = _bottom_right;

		return *this;
	}

	cWidget_gui& cWidget_gui::cornerRadius( const float _top, const float _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius.topLeft     = _top;
		m_data.cornerRadius.topRight    = _top;
		m_data.cornerRadius.bottomLeft  = _bottom;
		m_data.cornerRadius.bottomRight = _bottom;

		return *this;
	}

	cWidget_gui& cWidget_gui::cornerRadius( const float _radius )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius.topLeft     = _radius;
		m_data.cornerRadius.topRight    = _radius;
		m_data.cornerRadius.bottomLeft  = _radius;
		m_data.cornerRadius.bottomRight = _radius;

		return *this;
	}

	cWidget_gui& cWidget_gui::image( cTexture2D* _texture )
	{
		DF_ProfilingScopeCpu;

		m_data.image.imageData         = _texture;
		m_data.aspectRatio.aspectRatio = static_cast< float >( _texture->getSize().width() ) / static_cast< float >( _texture->getSize().height() );

		return *this;
	}

	cWidget_gui& cWidget_gui::image( cTexture2D* _texture, const cVector2f& _size )
	{
		DF_ProfilingScopeCpu;

		m_data.image.imageData         = _texture;
		m_data.aspectRatio.aspectRatio = _size.width() / _size.height();

		return *this;
	}

	cWidget_gui& cWidget_gui::border( const cBorder_gui& _border )
	{
		DF_ProfilingScopeCpu;

		m_data.border = _border.get();

		return *this;
	}

	cWidget_gui& cWidget_gui::addChild( const cText_gui& _text )
	{
		DF_ProfilingScopeCpu;

		if( !_text.isEmpty() )
			m_text = _text;
		else
			m_text.reset();

		return *this;
	}

	cWidget_gui& cWidget_gui::addChild( const cWidget_gui& _child )
	{
		DF_ProfilingScopeCpu;

		m_children.push_back( _child );

		return *this;
	}

	cWidget_gui& cWidget_gui::addChildren( const std::vector< cWidget_gui >& _children )
	{
		DF_ProfilingScopeCpu;

		m_children.insert( m_children.end(), _children.begin(), _children.end() );

		return *this;
	}

	void cWidget_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		if( m_text.has_value() )
			m_text->paint();
		else
		{
			CLAY( m_data )
			{
				for( const cWidget_gui& child: m_children )
					child.paint();
			}
		}
	}
}