#pragma once

#include <clay.h>
#include <string>
#include <vector>

#include "cBorder_gui.h"
#include "cLayout_gui.h"
#include "engine/core/math/cVector.h"
#include "engine/core/utils/cColor.h"
#include "engine/core/utils/Misc.h"

namespace df
{
	class iTexture;
}

namespace df::gui
{
	class cWidget_gui
	{
	public:
		DF_DefaultCopyAndMove( cWidget_gui );

		cWidget_gui();
		cWidget_gui( const std::string& _id );
		~cWidget_gui() = default;

		cWidget_gui& id( const std::string& _id );

		cWidget_gui& layout( const cLayout_gui& _layout );

		cWidget_gui& color( const cColor& _color );

		cWidget_gui& cornerRadius( float _top_left, float _top_right, float _bottom_left, float _bottom_right );
		cWidget_gui& cornerRadius( float _top, float _bottom );
		cWidget_gui& cornerRadius( float _radius );

		cWidget_gui& image( iTexture* _texture );
		cWidget_gui& image( iTexture* _texture, const cVector2f& _size );

		// cWidget_gui& floating();

		// cWidget_gui& custom();

		// cWidget_gui& scroll();

		cWidget_gui& border( const cBorder_gui& _border );

		// cWidget_gui& userData();

		cWidget_gui& addChild( const cWidget_gui& _child );
		cWidget_gui& addChildren( const std::vector< cWidget_gui >& _children );

		void paint() const;

		const Clay_ElementDeclaration& get() const { return m_data; }

	private:
		Clay_ElementDeclaration    m_data;
		std::vector< cWidget_gui > m_children;
	};
}
