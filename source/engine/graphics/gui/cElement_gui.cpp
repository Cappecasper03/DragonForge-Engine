#include "cElement_gui.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cElement_gui::cElement_gui()
		: m_data{}
	{}

	cElement_gui::cElement_gui( const std::string& _id )
		: m_data{}
	{
		DF_ProfilingScopeCpu;

		id( _id );
	}

	cElement_gui& cElement_gui::id( const std::string& _id )
	{
		DF_ProfilingScopeCpu;

		m_data.id = Clay__HashString( Clay_String{ .length = static_cast< int >( _id.size() ), .chars = _id.data() }, 0, 0 );

		return *this;
	}

	cElement_gui& cElement_gui::layout( const cLayout_gui& _layout )
	{
		DF_ProfilingScopeCpu;

		m_data.layout = _layout.get();

		return *this;
	}

	cElement_gui& cElement_gui::color( const cColor& _color )
	{
		DF_ProfilingScopeCpu;

		m_data.backgroundColor.r = _color.r;
		m_data.backgroundColor.g = _color.g;
		m_data.backgroundColor.b = _color.b;
		m_data.backgroundColor.a = _color.a;

		return *this;
	}

	cElement_gui& cElement_gui::cornerRadius( const float _top_left, const float _top_right, const float _bottom_left, const float _bottom_right )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius.topLeft     = _top_left;
		m_data.cornerRadius.topRight    = _top_right;
		m_data.cornerRadius.bottomLeft  = _bottom_left;
		m_data.cornerRadius.bottomRight = _bottom_right;

		return *this;
	}

	cElement_gui& cElement_gui::cornerRadius( const float _top, const float _bottom )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius.topLeft     = _top;
		m_data.cornerRadius.topRight    = _top;
		m_data.cornerRadius.bottomLeft  = _bottom;
		m_data.cornerRadius.bottomRight = _bottom;

		return *this;
	}

	cElement_gui& cElement_gui::cornerRadius( const float _radius )
	{
		DF_ProfilingScopeCpu;

		m_data.cornerRadius.topLeft     = _radius;
		m_data.cornerRadius.topRight    = _radius;
		m_data.cornerRadius.bottomLeft  = _radius;
		m_data.cornerRadius.bottomRight = _radius;

		return *this;
	}

	cElement_gui& cElement_gui::border( const cBorder_gui& _border )
	{
		DF_ProfilingScopeCpu;

		m_data.border = _border.get();

		return *this;
	}

	cElement_gui& cElement_gui::addChild( const cElement_gui& _child )
	{
		DF_ProfilingScopeCpu;

		m_children.push_back( _child );

		return *this;
	}

	cElement_gui& cElement_gui::addChildren( const std::vector< cElement_gui >& _children )
	{
		DF_ProfilingScopeCpu;

		m_children.insert( m_children.end(), _children.begin(), _children.end() );

		return *this;
	}

	void cElement_gui::paint() const
	{
		DF_ProfilingScopeCpu;

		CLAY( m_data )
		{
			for( const cElement_gui& child: m_children )
				child.paint();
		}
	}
}