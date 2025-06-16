#pragma once

#include "engine/core/utils/cSmartPointers.h"
#include "engine/core/utils/iSingleton.h"
#include "engine/graphics/cameras/cRenderTextureCamera2D.h"

namespace df
{
	class cCameraManager final : public iSingleton< cCameraManager >
	{
	public:
		DF_DeleteCopyAndMove( cCameraManager );

		cCameraManager();
		~cCameraManager() override = default;

		cCamera* m_current;
		bool     m_current_is_regular;

		cUnique< cRenderTextureCamera2D > m_deferred_camera;
		cUnique< cCamera >                m_camera_main;
		cUnique< cCamera >                m_camera_gui;
	};
}
