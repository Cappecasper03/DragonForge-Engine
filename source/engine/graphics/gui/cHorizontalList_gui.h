#pragma once

#include "core/cWidget_gui.h"
#include "core/iWidget_gui.h"
#include "iList_gui.h"

namespace df::gui
{
	class cHorizontalList_gui : public iList_gui< cHorizontalList_gui >
	{
	public:
		DF_DefaultCopyAndMove( cHorizontalList_gui );

		cHorizontalList_gui()           = default;
		~cHorizontalList_gui() override = default;

		cShared< cHorizontalList_gui > alignment( cWidget_gui::eHorizontalAlignment _alignment );

	private:
		void initialize() override;
	};
}
