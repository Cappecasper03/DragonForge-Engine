#pragma once

#include "core/cWidget_gui.h"
#include "core/iWidget_gui.h"
#include "iList_gui.h"

namespace df::gui
{
	class cVerticalList_gui : public iList_gui< cVerticalList_gui >
	{
	public:
		DF_DefaultCopyAndMove( cVerticalList_gui );

		cVerticalList_gui()           = default;
		~cVerticalList_gui() override = default;

		cShared< cVerticalList_gui > alignment( cWidget_gui::eVerticalAlignment _alignment );

		void paint() const override;

	private:
		void initialize() override;
	};
}
