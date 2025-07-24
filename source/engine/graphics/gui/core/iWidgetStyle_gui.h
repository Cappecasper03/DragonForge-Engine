#pragma once

#include "cWidget_gui.h"
#include "engine/core/utils/Misc.h"

namespace df::gui
{
	struct sWidgetBrush
	{
		cShared< cTexture2D > image;
		cVector2f             image_size;

		cColor    color;
		cVector4u padding;
		cColor    border_color;
		cVector4u border_width;
		cVector4f corner_radius;
	};

	class iWidgetStyle_gui : public std::enable_shared_from_this< iWidgetStyle_gui >
	{
		friend class iWidget_gui;

	public:
		DF_DefaultCopyAndMove( iWidgetStyle_gui );

		iWidgetStyle_gui()          = default;
		virtual ~iWidgetStyle_gui() = default;

		const Clay_ElementDeclaration& get() const { return m_data.get(); }

	protected:
		virtual void update( const cShared< iWidget_gui >& _widget ) {}

		void applyBrush( const sWidgetBrush& _brush );

		cWidget_gui m_data;
	};
}
