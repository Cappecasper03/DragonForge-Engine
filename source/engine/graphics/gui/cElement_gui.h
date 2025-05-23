#pragma once

#include <clay.h>
#include <string>
#include <vector>

#include "cBorder_gui.h"
#include "cLayout_gui.h"
#include "engine/core/utils/cColor.h"
#include "engine/core/utils/Misc.h"

namespace df::gui
{
	class cElement_gui
	{
	public:
		DF_DefaultCopyAndMove( cElement_gui );

		cElement_gui();
		cElement_gui( const std::string& _id );
		~cElement_gui() = default;

		cElement_gui& id( const std::string& _id );

		cElement_gui& layout( const cLayout_gui& _layout );

		cElement_gui& color( const cColor& _color );

		cElement_gui& cornerRadius( float _top_left, float _top_right, float _bottom_left, float _bottom_right );
		cElement_gui& cornerRadius( float _top, float _bottom );
		cElement_gui& cornerRadius( float _radius );

		// cElement_gui& image();

		// cElement_gui& floating();

		// cElement_gui& custom();

		// cElement_gui& scroll();

		cElement_gui& border( const cBorder_gui& _border );

		// cElement_gui& userData();

		cElement_gui& addChild( const cElement_gui& _child );
		cElement_gui& addChildren( const std::vector< cElement_gui >& _children );

		void paint() const;

		const Clay_ElementDeclaration& get() const { return m_data; }

	private:
		Clay_ElementDeclaration     m_data;
		std::vector< cElement_gui > m_children;
	};
}
