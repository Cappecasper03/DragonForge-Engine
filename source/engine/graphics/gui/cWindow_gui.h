#pragma once

#include "base/cOverlay_gui.h"
#include "base/cVerticalList_gui.h"
#include "core/iWidget_gui.h"

namespace df::gui
{
	class cWindow_gui : public iWidgetCreate_gui< cWindow_gui >
	{
	public:
		DF_DefaultCopyAndMove( cWindow_gui );

		cWindow_gui()           = default;
		~cWindow_gui() override = default;

		cShared< cWindow_gui > setContent( const cShared< iWidget_gui >& _widget );

		void paint() const override { m_overlay->paint(); }

	private:
		void initialize() override;

		cShared< cOverlay_gui >      m_overlay;
		cShared< cVerticalList_gui > m_content;
	};
}
