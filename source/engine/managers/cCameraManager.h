#pragma once

#include "assets/iAssetManager.h"
#include "engine/graphics/cameras/cCamera.h"

namespace df
{
	class cCameraManager final : public iAssetManager< cCameraManager, cCamera >
	{
	public:
		DF_DisableCopyAndMove( cCameraManager );

		cCameraManager();
		~cCameraManager() override = default;

		cCamera* m_current;
	};
}
