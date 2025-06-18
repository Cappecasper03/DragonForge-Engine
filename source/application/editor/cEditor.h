#pragma once

#include "engine/core/utils/iSingleton.h"
#include "engine/graphics/cameras/cFreeFlightCamera.h"
#include "engine/graphics/gui/base/cHorizontalList_gui.h"

namespace df
{
	class cEditor : public iSingleton< cEditor >
	{
	public:
		DF_DeleteCopyAndMove( cEditor );

		cEditor();
		~cEditor() override = default;

		void update( float _delta_time );
		void render3D();
		void renderGui();

		cUnique< cFreeFlightCamera > m_camera;

		cShared< gui::cHorizontalList_gui > m_widget;
	};
}
