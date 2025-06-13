#pragma once

#include "engine/core/utils/iSingleton.h"

namespace df
{
	class cRenderTextureCamera2D;
	class cCamera;

	class cCameraManager final : public iSingleton< cCameraManager >
	{
	public:
		DF_DeleteCopyAndMove( cCameraManager );

		cCameraManager();
		~cCameraManager() override;

		cCamera* m_current;
		bool     m_current_is_regular;

		cRenderTextureCamera2D* m_deferred_camera;
		cCamera*                m_camera_main;
		cCamera*                m_camera_gui;
	};
}
