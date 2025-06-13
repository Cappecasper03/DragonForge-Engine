#pragma once

#include <unordered_map>

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

		std::unordered_map< std::string, cRenderTextureCamera2D* > m_texture_cameras;
		cCamera*                                                   m_camera_main;
		cCamera*                                                   m_camera_gui;
	};
}
