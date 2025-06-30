#pragma once

#include "base/cHorizontalList_gui.h"
#include "core/iWidget_gui.h"
#include "engine/core/utils/Misc.h"

namespace df::gui
{
	class cTitleBar_gui : public iWidgetCreate_gui< cTitleBar_gui >
	{
	public:
		DF_DefaultCopyAndMove( cTitleBar_gui );

		cTitleBar_gui();
		~cTitleBar_gui() override = default;

		void paint() const override { m_content->paint(); }

		bool canMove() const { return m_moving; }
		bool wasButtonDownThisFrame();

	private:
		void initialize() override;

		cShared< cHorizontalList_gui > m_content;

		bool m_moving;
		bool m_button_down;
	};
}
