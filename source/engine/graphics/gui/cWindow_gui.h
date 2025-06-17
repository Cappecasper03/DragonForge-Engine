#pragma once

#include "core/cWidget_gui.h"
#include "core/iWidget_gui.h"

namespace df::gui
{
	class cWindow_gui : public iWidgetCreate_gui< cWindow_gui >
	{
	public:
		DF_DefaultCopyAndMove( cWindow_gui );

		cWindow_gui()           = default;
		~cWindow_gui() override = default;

		cShared< cWindow_gui > setContent( cShared< iWidget_gui > _content );

		void paint() const override;

	private:
		void initialize() override;

		cWidget_gui m_data;
	};
}
