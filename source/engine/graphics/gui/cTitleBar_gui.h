#pragma once

#include "base/cButton_gui.h"
#include "base/cHorizontalList_gui.h"
#include "core/iWidget_gui.h"
#include "engine/core/utils/Misc.h"

namespace df::gui
{
	class cTitleBar_gui : public iWidgetCreate_gui< cTitleBar_gui >
	{
	public:
		DF_DefaultCopyAndMove( cTitleBar_gui );

		cTitleBar_gui()           = default;
		~cTitleBar_gui() override = default;

		void onDragStart( const std::function< void() >& _function ) { m_on_drag_start.subscribe( _function ); }
		void onDragEnd( const std::function< void() >& _function ) { m_on_drag_end.subscribe( _function ); }

		void onClose( const std::function< void() >& _function ) { m_on_close.subscribe( _function ); }

		void paint() const override { m_content->paint(); }

	private:
		void initialize() override;

		cShared< cHorizontalList_gui > m_content;
		cShared< cButton_gui >         m_close_button;

		cEvent<> m_on_close;

		cEvent<> m_on_drag_start;
		cEvent<> m_on_drag_end;
	};
}
